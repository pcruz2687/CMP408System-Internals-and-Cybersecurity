<?php
  include("config.php");
  session_start();

  // Check if user is logged in, if so redirect to control page
	if(isset($_SESSION['user_id'])) {
		header("location: index.php");
		exit;
  }
  
  	// Authenticate account details on form submit
	if(isset($_POST['register'])) {
		$username = $_POST['username'];
		$password = password_hash($_POST['password'], PASSWORD_DEFAULT);

		// Prepare SQL statements to prevent SQL injections
		$verify = $connection->prepare("SELECT COUNT(*) FROM users WHERE username=:username");
		$verify->bindParam("username", $username, PDO::PARAM_STR);
		$verify->execute([':username' => $username]);

		// Check if username exists, otherwise insert
		if($verify->fetchColumn()) {
      echo "Username already in use.";
    } else {
      $register = $connection->prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
      $register->bindParam("username", $username, PDO::PARAM_STR);
      $register->bindParam("password", $password, PDO::PARAM_STR);
      $register->execute();
      header("location: login.php");
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
  Register
  <form action="" method="post">
		<label for="username">Username</label><br>
		<input type="text" id="username" name="username" required/><br>
		<label for="password">Password</label><br>
		<input type="password" id="password" name="password" required/><br>
		<button type="submit" name="register" value="register">Register</button>
	</form>
</body>
</html>