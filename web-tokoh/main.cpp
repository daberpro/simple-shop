#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <httplib.h>
#include <mysql/jdbc.h>
#include <thread>
#include <filesystem>
#include <cstdarg>
#include <string.h>
#include <format>
#include <future>
#include <json.hpp>
#include <fstream>
#include <optional>
#include <algorithm>
#include <vector>
#include <tuple>


namespace{
	template<typename T = const char*>
	void log(uint32_t total_args, ...){
		va_list list;
		va_start(list,total_args);
		for(uint32_t n = 0; n < total_args; n++){
			std::cout << va_arg(list,T) << " ";
		}
		va_end(list);
		std::cout << "\n";
	}
	
	std::string __dir = std::filesystem::current_path().generic_string();
}


int main(int argc, char **argv)
{
	//setup configuration
	nlohmann::json configuration;
	try{
		if(std::filesystem::exists(__dir+"/config.json")){
			std::ifstream config(__dir+"/config.json");
			configuration = nlohmann::json::parse(config);
			config.close();
		}else{
			std::cout << "Cannot find configuration file config.json\n";
			exit(1);
		}
	}catch(nlohmann::json::exception& ex){
		std::cout << ex.what() << "\n";
		exit(1);
	}
	
	//setup database configuration	
	sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
	sql::Connection* connection = driver->connect(
		configuration["database_config"]["database_url"].get<std::string>(),
		configuration["database_config"]["user"].get<std::string>(),
		configuration["database_config"]["password"].get<std::string>()
	);
	connection->setAutoCommit(true);
	connection->setSchema(configuration["database_config"]["database_name"].get<std::string>());
	
	sql::Statement* cmd = connection->createStatement();
	sql::ResultSet* resultDB;
	sql::ResultSetMetaData* metaData;
	
	//server setup
	httplib::Server mServer;
	
	//error site
	std::string error_path = __dir+configuration["server_config"]["error_web"].get<std::string>();
	mServer.set_error_handler([&](const auto& req, auto& res){
		if(std::filesystem::exists(error_path)){
			std::ifstream html(error_path);
			std::string raw,result;
			while(getline(html,raw)){
				result += raw;
			}
			html.close();
			std::string error_template = "{{code}}";
			uint32_t error_template_pos = result.find(error_template);
			if(error_template_pos != result.size()){
				res.set_content(
					result.replace(error_template_pos,error_template.size(),std::to_string(res.status)),
					"text/html"
				);
			}else{
				res.set_content(
					result,
					"text/html"
				);
			}
		}else{
			res.set_content(
			"<h1>Error Code "+std::to_string(res.status)+" </h1>"
			,"text/html");
		}
	});
	
	//static site
	nlohmann::json static_web = configuration["server_config"]["static_web"];
	std::vector<nlohmann::json> conf = static_web.get<std::vector<nlohmann::json>>();
	for(auto& d: conf){
		auto web = mServer.set_mount_point(d["route"].get<std::string>(),__dir+d["path"].get<std::string>());
		if(!web){
			log(1,"Static dir not found ");
			exit(1);
		}
	}
	
	//routing
	mServer.Get("/about-dev",[&](const httplib::Request& req, httplib::Response& res){
		res.set_content(
			static_cast<std::string>(nlohmann::json({
				{"status", "Running"},
				{"version", "1.0"},
				{"backend", "C++"},
				{"cerateBy", "Ari Susanto"}
			}).dump(4)),
			"application/json"
		);
	});
	
	mServer.Get("/api-v1.0/:table/:limit/:offset",[&](const httplib::Request& req, httplib::Response& res){
		try{
			std::vector<std::string> public_data = configuration["database_config"]["public_data"].get<std::vector<std::string>>();
			if(std::find(public_data.begin(), public_data.end(),req.path_params.at("table")) != public_data.end()){
				nlohmann::json dataResult;
				nlohmann::json data_col;
				std::string columnName = "",
				table = req.path_params.at("table"),
				limit = req.path_params.at("limit"),
				offset = req.path_params.at("offset");
				
				//check limit is reached the maximum
				if(stoi(limit) > 30){
					limit = "30";
				}
				
				resultDB = cmd->executeQuery(
					"SELECT * FROM "+ table+
					" LIMIT " + limit+
					" OFFSET " + offset
				);
				metaData = resultDB->getMetaData();
				while(resultDB->next()){
					data_col.clear();
					unsigned int total_column = metaData->getColumnCount();
					for(unsigned int i = 1; i <= total_column; i++){
						columnName = metaData->getColumnName(i);
						data_col[columnName] = resultDB->getString(columnName);
					}
					dataResult.push_back(data_col);
				}
				res.set_content(
					static_cast<std::string>(dataResult.dump(4)),
					"application/json"
				);
			}else{
				res.set_content(
					static_cast<std::string>(nlohmann::json({
						{"Message", "public data not found"}
					}).dump(4)),
					"application/json"
				);
			}
		}catch(sql::SQLException ex){
			res.set_content(
				static_cast<std::string>(nlohmann::json({
					{"Message", ex.what()}
				}).dump(4)),
				"application/json"
			);
		}
	});
	
	mServer.Post("/api-v1.0/update/:table",[&](const httplib::Request& req, httplib::Response& res){
		try{
			//check for Content-Type of data
			if(req.get_header_value("Content-Type") != "application/json"){
				res.status = 400;
				res.set_content(
				static_cast<std::string>(nlohmann::json({
					{"Message","Bad Request"},
					{"Info","content type must be application/json"}
				}).dump(4))
				,"application/json");
				return;
			}
			std::string tableName = req.path_params.at("table");
			nlohmann::json data = nlohmann::json::parse(req.body);
			if(!(data.contains("fields") && data.contains("field_id"))){
				res.status = 400;
				res.set_content(
				static_cast<std::string>(nlohmann::json({
					{"Message","Bad Request"},
					{"Info","content type must be application/json"}
				}).dump(4))
				,"application/json");
			}
			
			std::vector<std::string> public_data = configuration["database_config"]["public_data"].get<std::vector<std::string>>();
			if(std::find(public_data.begin(), public_data.end(),tableName) != public_data.end()){
				try{
					sql::PreparedStatement* pStatement = connection->prepareStatement(
						" UPDATE "+ tableName +
						" SET " + data["fields"].get<std::string>() +
						" WHERE " + data["field_id"].get<std::string>()
					);
					for(auto& [key,value]: data.items()){
						if(key != "fields" && key != "field_id"){
							std::tuple<int,std::string> rValue = value.get<std::tuple<int,std::string>>();
							pStatement->setString(std::get<0>(rValue),std::get<1>(rValue));
						}
					}
					if(!pStatement->execute()){
						res.set_content(
						static_cast<std::string>(nlohmann::json({
							{"Message","Success to update"}
						}).dump(4))
						,"application/json");
					}else{
						res.set_content(
						static_cast<std::string>(nlohmann::json({
							{"Message","Failed to update"}
						}).dump(4))
						,"application/json");
					}
					delete pStatement;
				}catch(sql::SQLException ex){
					res.set_content(
					static_cast<std::string>(nlohmann::json({
						{"Message",ex.what()}
					}).dump(4))
					,"application/json");
				}
				
			}
			
		}catch(nlohmann::json::exception ex){
			res.set_content(
			static_cast<std::string>(nlohmann::json({
				{"Message",ex.what()},
				{"Info","theres look like your body data was broken"}
			}).dump(4))
			,"application/json");
		}
	});
	
	mServer.Post("/api-v1.0/insert/:table",[&](const httplib::Request& req, httplib::Response& res){
		try{
			//check for Content-Type of data
			if(req.get_header_value("Content-Type") != "application/json"){
				res.status = 400;
				res.set_content(
				static_cast<std::string>(nlohmann::json({
					{"Message","Bad Request"},
					{"Info","content type must be application/json"}
				}).dump(4))
				,"application/json");
				return;
			}
			std::string tableName = req.path_params.at("table");
			nlohmann::json data = nlohmann::json::parse(req.body);
			if(!(data.contains("fields") && data.contains("fields_value"))){
				res.status = 400;
				res.set_content(
				static_cast<std::string>(nlohmann::json({
					{"Message","Bad Request"},
					{"Info","content type must be application/json"}
				}).dump(4))
				,"application/json");
			}
			
			std::vector<std::string> public_data = configuration["database_config"]["public_data"].get<std::vector<std::string>>();
			if(std::find(public_data.begin(), public_data.end(),tableName) != public_data.end()){
				try{
					sql::PreparedStatement* pStatement = connection->prepareStatement(
						" INSERT INTO "+ tableName +
						data["fields"].get<std::string>() +
						" VALUES " + data["fields_value"].get<std::string>()
					);
					for(auto& [key,value]: data.items()){
						if(key != "fields" && key != "fields_value"){
							std::tuple<int,std::string> rValue = value.get<std::tuple<int,std::string>>();
							pStatement->setString(std::get<0>(rValue),std::get<1>(rValue));
						}
					}
					if(!pStatement->execute()){
						res.set_content(
						static_cast<std::string>(nlohmann::json({
							{"Message","Success to insert"}
						}).dump(4))
						,"application/json");
					}else{
						res.set_content(
						static_cast<std::string>(nlohmann::json({
							{"Message","Failed to insert"}
						}).dump(4))
						,"application/json");
					}
					delete pStatement;
				}catch(sql::SQLException ex){
					res.set_content(
					static_cast<std::string>(nlohmann::json({
						{"Message",ex.what()}
					}).dump(4))
					,"application/json");
				}
				
			}
			
		}catch(nlohmann::json::exception ex){
			res.set_content(
			static_cast<std::string>(nlohmann::json({
				{"Message",ex.what()},
				{"Info","theres look like your body data was broken"}
			}).dump(4))
			,"application/json");
		}
	});
	
	mServer.Post("/api-v1.0/delete/:table",[&](const httplib::Request& req, httplib::Response& res){
		try{
			//check for Content-Type of data
			if(req.get_header_value("Content-Type") != "application/json"){
				res.status = 400;
				res.set_content(
				static_cast<std::string>(nlohmann::json({
					{"Message","Bad Request"},
					{"Info","content type must be application/json"}
				}).dump(4))
				,"application/json");
				return;
			}
			std::string tableName = req.path_params.at("table");
			nlohmann::json data = nlohmann::json::parse(req.body);
			if(!(data.contains("fields"))){
				res.status = 400;
				res.set_content(
				static_cast<std::string>(nlohmann::json({
					{"Message","Bad Request"},
					{"Info","content type must be application/json"}
				}).dump(4))
				,"application/json");
			}
			
			std::vector<std::string> public_data = configuration["database_config"]["public_data"].get<std::vector<std::string>>();
			if(std::find(public_data.begin(), public_data.end(),tableName) != public_data.end()){
				try{
					sql::PreparedStatement* pStatement = connection->prepareStatement(
						" DELETE FROM "+ tableName +
						" WHERE " + data["fields"].get<std::string>()
					);
					for(auto& [key,value]: data.items()){
						if(key != "fields"){
							std::tuple<int,std::string> rValue = value.get<std::tuple<int,std::string>>();
							pStatement->setString(std::get<0>(rValue),std::get<1>(rValue));
						}
					}
					if(!pStatement->execute()){
						res.set_content(
						static_cast<std::string>(nlohmann::json({
							{"Message","Success to delete"}
						}).dump(4))
						,"application/json");
					}else{
						res.set_content(
						static_cast<std::string>(nlohmann::json({
							{"Message","Failed to delete"}
						}).dump(4))
						,"application/json");
					}
					delete pStatement;
				}catch(sql::SQLException ex){
					res.set_content(
					static_cast<std::string>(nlohmann::json({
						{"Message",ex.what()}
					}).dump(4))
					,"application/json");
				}
				
			}
			
		}catch(nlohmann::json::exception ex){
			res.set_content(
			static_cast<std::string>(nlohmann::json({
				{"Message",ex.what()},
				{"Info","theres look like your body data was broken"}
			}).dump(4))
			,"application/json");
		}
	});
	
	std::thread restAPI([&](){
		log(2,"Server runnin on PORT",std::to_string(configuration["server_config"]["server_port"].get<uint32_t>()).c_str());
		mServer.listen(
			configuration["server_config"]["server_host"].get<std::string>(),
			configuration["server_config"]["server_port"].get<uint32_t>()
		);
	});
	
	log(1,"Enter to close program\n");
	std::cin.get();
	mServer.stop();
	restAPI.join();
	
	delete connection;
	delete cmd;
	delete resultDB;
	
	return 0; 
}
