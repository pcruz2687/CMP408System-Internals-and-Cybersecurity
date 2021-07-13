<?php
  // MySQL Database Credentials
  define('USER', 'admin');
  define('PASSWORD', 'adminpass');
  define('HOST', 'aae50jppq99xii.cb2mjdifr9ad.us-east-1.rds.amazonaws.com');
  define('DATABASE', 'dbcmp408');

  // Connect to MySQL Database
  try {
    $connection = new PDO("mysql:host=".HOST.";dbname=".DATABASE, USER, PASSWORD);
  } catch (PDOException $e) {
    exit("Error: " . $e->getMessage());
  }
?>