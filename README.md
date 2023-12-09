# Simple Shop
### c++ project
> simple project to making CRUD with mysql databases\
> Create by Ari Susanto aka Daberdev/Daberpro
Dependencies
- libhttp
- mysql c++ conector
- nlohmann/json

IDE
- CodeLite IDE

Compilers
- MSVC Toolset 2022

Language server
- LLVM 17.0.1

Configuration file `config.json`
```json
{
  "server_config": {
      "server_name": "web tokoh",
      "server_version": "1.0.0",
      "server_port": 3000,
      "server_host": "0.0.0.0",
      "static_web": [{
          "route": "/",
          "path": "/public"
      }],
      "error_web": "/public/error/index.html"
  },
  "database_config": {
      "database_url": "tcp://127.0.0.1:3306",
      "database_name": "restapi",
      "user": "root",
      "password": "",
      "public_data": [
          "barang"
      ]
  }
}
```
*server config*: all configurtion for server, such as IP and PORT for binding and static web\
*database config*: all configuration for mysql c++ connector\
`public_data` is tabel in database
