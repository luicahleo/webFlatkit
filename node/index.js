var mysql = require('mysql');//estos dos hay que instalarlo en nuestro VPS dentro de node, porque es solo para este [Proyecto]
var mqtt = require('mqtt');

//CREDENCIALES MYSQL
var con = mysql.createConnection({
  host: "timbrewifi.ga",
  user: "admin_cursoiot",
  password: "121212",
  database: "admin_cursoiot"
});

//CREDENCIALES MQTT
var options = {
  port: 1883, //puerto directo TCP
  host: 'timbrewifi.ga',
  clientId: 'acces_control_server_' + Math.round(Math.random() * (0- 10000) * -1) ,//generamos numero aleatorios con cabecera acces_control_server_
  username: 'web_client',
  password: '121212',
  keepalive: 60,//
  reconnectPeriod: 1000,//si se cae, intento cada 1000 ms
  protocolId: 'MQIsdp',
  protocolVersion: 3,
  clean: true,
  encoding: 'utf8'
};

var client = mqtt.connect("mqtt://timbrewifi.ga", options);//este es el cliente

//SE REALIZA LA CONEXION
client.on('connect', function () {
  console.log("Conexión  MQTT Exitosa!");
  client.subscribe('+/#', function (err) {//nos suscribimos al topico 'device'
    console.log("Subscripción exitosa!")
  });
})

//CUANDO SE RECIBE MENSAJE
client.on('message', function (topic, message) {
  console.log("Mensaje recibido desde -> " + topic + " Mensaje -> " + message.toString());
  if (topic == "values"){
    var msg = message.toString();
    var sp = msg.split(",");
    var temp1 = sp[0];
    var temp2 = sp[1];
    var volts = sp[2];

    //hacemos la consulta para insertar....
    var query = "INSERT INTO `admin_cursoiot`.`data` (`data_temp1`, `data_temp2`, `data_volts`) VALUES (" + temp1 + ", " + temp2 + ", " + volts + ");";
    con.query(query, function (err, result, fields) {
      if (err) throw err;
      console.log("Fila insertada correctamente");
    });
  }
});




//nos conectamos
con.connect(function(err){
  if (err) throw err;//si hay error, hace lo de abajo

  //una vez conectados, podemos hacer consultas.
  console.log("Conexión a MYSQL exitosa!!!")

  //hacemos la consulta
  var query = "SELECT * FROM devices WHERE 1";
  con.query(query, function (err, result, fields) {
    if (err) throw err;
    if(result.length>0){//si el tamano del result es mayor a 0
      console.log(result);
    }
  });

});



//para mantener la sesión con mysql abierta
setInterval(function () {//este metodo, ejecuta lo que hay dentro cada 5000 ms
  var query ='SELECT 1 + 1 as result';//la consulta la guardamos en query como string

  con.query(query, function (err, result, fields) {
    if (err) throw err;
  });

}, 5000);
