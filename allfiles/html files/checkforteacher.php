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

$con = mysqli_connect('localhost','peter','abc123','my_db');
if (!$con) {
    die('Could not connect: ' . mysqli_error($con));
}

mysqli_select_db($con,"ajax_demo");
$sql="SELECT P.ADLT_ID, P.GROUP_ID, P.ADLT_PSWD, P.SDNT_PMN
    FROM ADULT_TBL P
    WHERE P.ADLT_ID like '".$q."'";
$result = mysqli_query($con,$sql);

echo "<table>
<tr>
<th>ADLT_ID</th>
<th>GROUP_ID</th>
<th>ADLT_PSWD</th>
<th>SDNT_PMN</th>
</tr>";
while($row = mysqli_fetch_array($result)) {
    echo "<tr>";
    echo "<td>" . $row['ADLT_ID'] . "</td>";
    echo "<td>" . $row['GROUP_ID'] . "</td>";
    echo "<td>" . $row['ADLT_PSWD'] . "</td>";
    echo "<td>" . $row['SDNT_PMN'] . "</td>";
    echo "</tr>";
}
echo "</table>";
mysqli_close($con);
?>
</body>
</html>