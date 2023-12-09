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

### Endpoint (frontend)
for example of endpoint function, let's say we have database called restapi\
and tables inside of it called barang
***

to get all data from spesific table\
**NOTE:**
> the url target must be `http://HOST:PORT/api-v1.0/:TABLE_NAME/:COUNT/:START_COUNT`\
> HOST and PORT is the IP and the PORT where's server running\
> :TABLE_NAME is the table in database\
> :COUNT is total rows will show\
> :START_COUNT is index of row will start to show\
```javascript
fetch("http://127.0.0.1:3000/api-v1.0/barang/2/1",{
  method: "GET"
})
.then(d => d.json())
.then(console.log)
```
to update table.\
**NOTE:**
> the fields and field_id key inside the body JSON is important\
> `fields:` mean the field in table you wanna update\
> and `field_id` is the primary key of the table\
> rest of it just a `key:value` of fields and field_id\

```javascript
fetch("http://127.0.0.1:3000/api-v1.0/update/barang",
  {
    method:"POST",
    headers:{
      "Content-Type":"application/json"
    },
    body:JSON.stringify({
      fields: "jumlah=?, nama=?, tanggal_masuk=?",
      field_id: "id=?",
      jumlah: [1,"100"],
      nama: [2,"sabun"],
      tanggal_masuk: [3,(new Date()).toISOString("YYYY-MM-DD").split("T")[0]], 
      id: [4,"1"]
    })
  }
)
.then(d => d.json())
.then(console.log)

```
to insert rows in table
```javascript
fetch("http://127.0.0.1:3000/api-v1.0/insert/barang",
  {
    method:"POST",
    headers:{
      "Content-Type":"application/json"
    },
    body:JSON.stringify({
      fields: "(nama,jumlah,harga,tanggal_masuk)",
      fields_value: "(?,?,?,?)",
      nama: [1,"Sunlight"],
      jumlah: [2,"150"],
      tanggal_masuk: [4,(new Date()).toISOString("YYYY-MM-DD").split("T")[0]], 
      harga: [3,"2250000"]
    })
  }
)
.then(d => d.json())
.then(console.log)
```
to delete rows in table
```javascript
fetch("http://127.0.0.1:3000/api-v1.0/delete/barang",
  {
    method:"POST",
    headers:{
      "Content-Type":"application/json"
    },
    body:JSON.stringify({
      fields: "id=?",
      id: [1,"1"]
    })
  }
)
```
