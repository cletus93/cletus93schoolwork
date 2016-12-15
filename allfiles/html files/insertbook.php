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
$u = intval($_GET['u']);
$v = intval($_GET['v']);
$w = intval($_GET['w']);
$x = intval($_GET['x']);
$y = intval($_GET['y']);
$z = intval($_GET['z']);
$a = intval($_GET['a']);

$con = mysqli_connect('localhost','peter','abc123','my_db');
if (!$con) {
    die('Could not connect: ' . mysqli_error($con));
}

mysqli_select_db($con,"ajax_demo");
$sql="INSERT_INTO BOOK_TBL (BK_TITLE, CHPTRS, PAGES, BOOK_ID, UPLOADER_ID, GROUP_ID, BK_FMT, GRADE_LVL, LANG, VOCAB_ID, STND_ID)
    VALUES '".$q."', '".$r."', '".$s."', '".$t."', '".$u."', '".$v."', '".$w."', '".$x."', '".$y."', '".$z."', '".$a."'";
$result = mysqli_query($con,$sql);

mysqli_close($con);
?>
</body>
</html>