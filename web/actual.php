<html><head>
        <title>Estacion Meteorologica</title>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <script type="text/javascript" src="http://cdnjs.cloudflare.com/ajax/libs/jquery/2.0.3/jquery.min.js"></script>
        <script type="text/javascript" src="http://netdna.bootstrapcdn.com/bootstrap/3.3.4/js/bootstrap.min.js"></script>
        <link href="http://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.3.0/css/font-awesome.min.css" rel="stylesheet" type="text/css">
        <link href="index3.css" rel="stylesheet" type="text/css">
    </head><body>
        <div class="section section-primary">
            <div class="container"><div class="row"><div class="col-md-12"><h1 class="text-center">Estación meteorológica<br><br>Visualización de datos en tiempo real</h1></div></div></div>
        </div>
        <h4 class="text-center">
            <?php 
			
			$conexion=mysqli_connect( "db_host",
            "db_user", "db_pass", "db_name") or die( "Problemas con la conexión");
            $numero=mysqli_query($conexion, "SELECT * FROM estacion ORDER BY numero DESC LIMIT 0 , 1") 
			or die( "Problemas en el select:".mysqli_error($conexion)); 
			//$extraido=mysqli_fetch_array($numero); 
			
			/* $maximo=$ extraido[ 'numero']; $datos=mysqli_query($conexion,
            "SELECT * FROM  `estacion` WHERE  `numero` ="$maximo " LIMIT 0 , 30") or die( "Problemas en el select:".mysqli_error($conexion));
            $informacion=m ysqli_fetch_array($fecha); echo $informacion[ 'fecha'] */
			
			
            while ($extraido=mysqli_fetch_array($numero)) { 
			
			echo
            "<b>Fecha y hora (UTC): ".$extraido[ 'fecha']. "<br>"; 
			echo "<b>Numero de registro: </b>".$extraido['numero']. "<br>"; 
			echo "<b>Ubicación de estación: </b>".$extraido[ 'id']."<br>"; 
			echo "<b>Temperatura LM 35: </b>".$extraido[ 'templm']." grados Celsius<br>"; 
			echo "<b>Temperatura DHT 11: </b>".$extraido[ 'tempdht']." grados Celsius<br>"; 
			echo "<b>Temperatura BMP 180: </b>".$extraido[ 'tempbmp']." grados Celsius<br>"; 
			echo "<b>Temperatura PROMEDIO: </b>".$extraido[ 'tempprom']." grados Celsius<br>"; 
			echo "<b>Presion Atmosferica: </b>".$extraido[ 'presion']." hPa<br>"; 
			echo "<b>Humedad ambiental: </b>".$extraido[ 'humedadamb']. " %<br>";
            echo "<b>Humedad del suelo 1: </b>".$extraido[ 'humedadsuelo1']." %<br>"; 
			echo "<b>Humedad del suelo 2: </b>".$extraido[ 'humedadsuelo2']." %<br>"; 
			echo "<b>Humedad del suelo 3: </b>".$extraido[ 'humedadsuelo3']." %<br>"; 
			echo "<b>Iluminancia: </b>".$extraido[ 'luz']. " lux<br>"; 
			echo"<b>Punto de rocio: </b>".$extraido[ 'procio']. " grados Celsius<br>"; 
			echo"<b>Velocidad del viento: </b>".$extraido[ 'velocidadviento']. " km/h<br>"; 
			echo"<b>Sensacion termica: </b>".$extraido[ 'stermica']. " grados Celsius<br>"; 
			echo "<br>"; 
			echo"<hr>"; } 
			mysqli_close($conexion); 
			?>
        </h4>
    

</body></html>