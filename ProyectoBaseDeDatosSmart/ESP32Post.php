<?php

include 'conexion.php';

if($con){
    echo "Conexion con base de datos exitosa";

    if(isset($_POST['orientationX'])){
        $orientationX = $_POST['orientationX'];
        echo " Orientation: ".$orientationX;
    }
    

    if(isset($_POST['orientationY'])){
        $orientationY = $_POST['orientationY'];
        echo " Orientation: ".$orientationY;
    }

    if(isset($_POST['temperatura'])){
        $temperatura = $_POST['temperatura'];
        echo " Temperatura: ".$temperatura;
    }

    if(isset($_POST['humedad'])){
        $humedad = $_POST['humedad'];
        echo " Humedad: ".$humedad;
    }

    if(isset($_POST['presion'])){
        $presion = $_POST['presion'];
        echo " Presion: ".$presion;
    }

    if(isset($_POST['altitud'])){
        $altitud = $_POST['altitud'];
        echo " Altitud: ".$altitud;

        //Colocamos la parte de la fecha actual:
        date_default_timezone_set('america/bogota');
        $fecha_actual = date("Y-m-d H:i:s");

        $consulta = "INSERT INTO Tb_DATOS(OrientacionX, OrientacionY, Temperatura, Humedad, Presion, Altitud, Fecha_actual) VALUES('$orientationX','$orientationY','$temperatura','$humedad','$presion','$altitud','$fecha_actual')";

        $resultado = mysqli_query($con, $consulta);
        if($resultado){
            echo " Registro en base de datos OK!";
        }else{
            echo " Falla! Registro BD";
        }
    }

}else{
    echo "Falla! conexion con Base de datos ";
}

?>