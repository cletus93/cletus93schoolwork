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
$s = intval($_GET['s']);
$t = intval($_GET['t']);

$con = mysqli_connect('localhost','peter','abc123','my_db');
if (!$con) {
    die('Could not connect: ' . mysqli_error($con));
}

mysqli_select_db($con,"ajax_demo");
$sql="INSERT_INTO ADULT_TBL (ADLT_ID, GROUP_ID, ADLT_PSWD, ADLT_PMN)
    VALUES ('".$q."', '".$r."', '".$s."', '".$t."')";
$result = mysqli_query($con,$sql);

mysqli_close($con);
?>
</body>
</html>