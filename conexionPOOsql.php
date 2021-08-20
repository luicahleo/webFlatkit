
<!DOCTYPE html>
<html lang="en" dir="ltr">
  <head>
    <meta charset="utf-8">
    <title>conexion BBDD</title>
  </head>
  <body>

    <?php

      $conexion = new mysqli("localhost","admin_timbrewifi","121212","admin_pruebas");
      if ($conexion->connect_errno) {
        echo "fallo la conexion" . $conexion->connect_errno;
      }

      //para codificacion
      $conexion->set_charset("utf-8");

      //senetncia
      $sql = "SELECT * FROM datospersonales";

      $resultados = $conexion->query($sql);

      if ($conexion->errno) {
        die($conexion->error);
      }

      while($fila=$resultados->fetch_assoc()) {

        echo "<table><tr><td>";

        echo $fila['id'] . "</td><td>";
        echo $fila['nombre'] . "</td><td>";
        echo $fila['apellido'] . "</td><td>";
        echo $fila['edad'] . "</td><td></tr></table>";

        echo "<br>";
        echo "<br>";


      }


      $conexion->close();
     ?>




  </body>
</html>
