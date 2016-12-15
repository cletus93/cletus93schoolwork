//This program does all the math for the ticket user interface program
<!DOCTYPE html>
<html>
	<head>
		<title>Math for widgets</title>
	</head>
	<body>
		<?php
			$w1 = $_POST["w1"];
			$w2 = $_POST["w2"];
			$w3 = $_POST["w3"];
			$state = $_POST["state"];
			
			if ($w1 < 0) print ("Please enter positive numbers.");
			if ($w2 < 0) print ("Please enter positive numbers.");
			if ($w3 < 0) print ("Please enter positive numbers.");
			if ($state == "") print ("Please enter your state.");

			if ($w1 == "") $w1 = 0;
			if ($w2 == "") $w2 = 0;
			if ($w3 == "") $w3 = 0;

			$w1_cost = 12.45 * $w1;
			$w2_cost = 15.34 * $w2;
			$w3_cost = 28.99 * $w3;
			$total_cost = $w1_cost + $w2_cost + $w3_cost;
			$total_items = $w1 + $w2 +$w3;
			?>

			if ($total_items > 40)
			{
				$discount = $total_cost * .05;
				print ("You recieved a discount of $$discount for ordering
more than fourty widgets.\n");
				$total_cost = $total_cost - $discount;
			}
			else
				print ("You did not recieve a discount.\n");
			
			if ($state == "MO")
			{	
				$tax = $total_cost * .04375;	
				print ("Sorry you have to pay the Missouri tax of $$tax.");
				$total_cost = $total_cost + $tax;
			}
			else 
				print("You did not have to pay additional tax.");
		
			print ("Your total is $$total_cost.");	
		?>
	
		<p /><p />
		<table border = "border">
			<caption>Order Information</caption>
			<tr>
				<th> Product </th>
				<th> Unit Price </th>
				<th> Quantity Ordered </th>
				<th> Item Cost </th>
			</tr>
			<tr align = "center">
				<td> model 37AX-L </td>
				<td> $12.45 </td>
				<td> <?php print ("$w1"); ?> </td>
				<td> <?php printf ("$ %4.2f", $w1_cost); ?> </td>
			</tr>
			<tr align = "center">
				<td> model 42CR-J </td>
				<td> $15.34 </td>
				<td> <?php print ("$w2"); ?> </td>
				<td> <?php printf ("$ %4.2f", $w2_cost); ?> </td>
			</tr>
			<tr align = "center">
				<td> model 93ZZ-A </td>
				<td> $28.99 </td>
				<td> <?php print ("$w3"); ?> </td>
				<td> <?php printf ("$ %4.2f", $w3_cost); ?> </td>
			</tr>
		</table>
		<p /><p />
	</body>
</html>
