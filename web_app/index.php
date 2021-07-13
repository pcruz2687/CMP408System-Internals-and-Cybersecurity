<?php
  // Initialise Session
  include("config.php");
  session_start();

  // Check if user is logged in, if not then redirect to login page
  if(!isset($_SESSION['user_id'])) {
    header("location: login.php");
    exit;
  }
  
  if(isset($_POST['logout'])) {
    session_destroy();
    header("location: login.php");
  }

  // Auto load the dependencies that PHP MQTT requires
  require_once 'vendor/autoload.php';

  // PHP MQTT Classes
  use PhpMqtt\Client\{ConnectionSettings};
  use PhpMqtt\Client\Exceptions\{MqttClientExcepton};
  use PhpMqtt\Client\{MqttClient};
  use Psr\Log\{LogLevel};

  // Requirements for secure MQTT
  $server = 'ec2-52-91-232-191.compute-1.amazonaws.com';
  $port = 8883;
  $client_ID = 'ClientPiCam-3';
  $client_username = "subclient";
  $client_password = "subclientpw";
  $mqtt_certificate = "mqtt_ca.crt";

  // Publish topic and message
  $topic = "Raspberry Pi Control";
  $message = "capture";
  $qos = 1;

  // Only allow image capture if logged in
  if(isset($_POST['capture']) && isset($_SESSION['user_id'])) {
    try {
      $connectionSettings = (new ConnectionSettings)
        ->setUseTls(true)
        ->setUsername($client_username)
        ->setPassword($client_password)
        ->setTlsSelfSignedAllowed(true)
        ->setTlsCertificateAuthorityFile($mqtt_certificate);

      $mqtt = new MqttClient($server, $port, $client_ID);
      $mqtt->connect($connectionSettings, true);
      $mqtt->publish($topic, $message, $qos);
      
    } catch (MqttClientException $e) {
      echo 'MQTT Client Exception: ' . $e;
    }
    $mqtt->disconnect();
  }

  // Only allow viewing of images if logged in
  if(isset($_POST['view-images'])  && isset($_SESSION['user_id'])) {
		$query = $connection->prepare("SELECT image_url FROM images LIMIT 10");
		$query->execute();

		$result = $query->fetchAll();

		if(!$result) {
			echo "An error has occurred fetching the images.";
		} else {
      for($i = 0; $i <= ceil(count($result) / 2); $i++) {
        echo "<img src='" . $result[$i]['image_url'] . "' style='width: 400px; height: 250px;'><span> - </span>";
      }
      echo "<br>";
      for($i = ceil(count($result) / 2) + 1; $i < count($result); $i++) {
        echo "<img src='" . $result[$i]['image_url'] . "' style='width: 400px; height: 250px;'><span> - </span>";
      }
		}
  }
?>

<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Document</title>
</head>
<body>
  <br>
  Take an image
  <form action="" method="post">
    <button type="submit" name="capture" value="capture">Capture</button>
  </form>
  <form action="" method="post">
    Display the 10 latest images</br>
    <button type="submit" name="view-images" value="view-images">View Images</button>
  </form>
  <form action="" method="post">
    <button type="submit" name="logout" value="logout">Log Out</button>
  </form>
</body>
</html>