<?php
  // Initialise Session
	include("config.php");
	session_start();

	// Check if user is logged in, if so redirect to control page
	if(isset($_SESSION['user_id'])) {
		header("location: index.php");
		exit;
	}

	// Authenticate account details on form submit
	if(isset($_POST['login'])) {
		$username = $_POST['username'];
		$password = $_POST['password'];

		// Prepare SQL statements to prevent SQL injections
		$query = $connection->prepare("SELECT * FROM users WHERE USERNAME=:username");
		$query->bindParam("username", $username, PDO::PARAM_STR);
		$query->execute();

		$result = $query->fetch(PDO::FETCH_ASSOC);

		if(!$result) {
			echo "Incorrect account details.";
		} else if ($username == $result['username']) {
			if(password_verify($password, $result['password'])) {
				// Create session on successful login
				$_SESSION['user_id'] = $result['id'];
				//echo $result['password'];
				header("location: index.php");
			} else {
				echo "Incorrect account details.";
			}
		}
	}
?>

<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Login</title>
</head>
<body>
	Log In
	<form action="" method="post">
		<label for="username">Username</label><br>
		<input type="text" id="username" name="username" required/><br>
		<label for="password">Password</label><br>
		<input type="password" id="password" name="password" required/><br>
		<button type="submit" name="login" value="login">Log In</button>
	</form>
</body>
</html>