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
$sql="SELECT BK_TITLE, CHPTRS, PAGES, BOOK_ID, UPLOADER_ID, GROUP_ID BK_FMT, GRADE_LVL, LANG, VOCAB_ID, STND_ID
    FROM BOOK_TBL
     WHERE GROUP_ID like '".$q."'";
$result = mysqli_query($con,$sql);

echo "<table>
<tr>
<th>STDNT_ID</th>
<th>ADULT_ID</th>
<th>STDNT_PSWD</th>
<th>SDNT_PMN</th>
</tr>";
while($row = mysqli_fetch_array($result)) {
    echo "<tr>";
    echo "<td>" . $row['STDNT_ID'] . "</td>";
    echo "<td>" . $row['ADULT_ID'] . "</td>";
    echo "<td>" . $row['STDNT_PSWD'] . "</td>";
    echo "<td>" . $row['SDNT_PMN'] . "</td>";
    echo "</tr>";
}
echo "</table>";
mysqli_close($con);
?>
</body>
</html>