<!DOCTYPE html>
<html>
<head>
<style>
table {
    width: 100%;
    border-collapse: collapse;
}

table, td, th {
    border: 1px solid black;
    padding: 5px;
}

th {text-align: left;}
</style>
</head>
<body>

<?php
$q = intval($_GET['q']);
$r = intval($_GET['r']);

$con = mysqli_connect('localhost','peter','abc123','my_db');
if (!$con) {
    die('Could not connect: ' . mysqli_error($con));
}

mysqli_select_db($con,"ajax_demo");
$sql="SELECT BK_TITLE
    FROM BOOK_TBL
      WHERE BK_TITLE like '".$q."', STND_ID like '".$r."'";
	  
$result = mysqli_query($con,$sql);

mysqli_close($con);
?>
</body>
</html>