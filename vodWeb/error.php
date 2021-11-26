<title>VoD Error</title>
<?php
	 switch ($_GET["errno"]) { 
	   case 1: 
		    echo "ACCESS Fails";
			echo '<meta http-equiv="refresh" content="2;URL=index.php">';
		   break; 
	   case 2: 
			echo "Undefined Access";
			//echo '<meta http-equiv="refresh" content="3;URL=index.php">';
		   break; 
	   case 3: 
	   		echo "You don't have authorization to see this page";
			echo '<meta http-equiv="refresh" content="2;URL=index.php">';
		   break; 
	   case 4: 
	   		echo "Password Fails";
			echo '<meta http-equiv="refresh" content="3;URL=index.php">';
		   break; 
		case 5: 
	   		echo "Username is already taken";
			echo '<meta http-equiv="refresh" content="3;URL=index.php">';
		   break; 
		/*case 6: 
	   		echo "You are NOT an Assistant";
			echo '<meta http-equiv="refresh" content="3;URL=index.php">';
		   break;
		case 7: 
	   		echo "You don't have access as Manager";
			echo '<meta http-equiv="refresh" content="3;URL=index.php">';
		   break; 
		case 8: 
	   		echo "This Reference code is already being used";
			echo '<meta http-equiv="refresh" content="3;URL=management/addcourse.php">';
		   break; 
		case 9: 
	   		echo "This Student Number is already being used";
			echo '<meta http-equiv="refresh" content="3;URL=management/addstudent.php">';
		   break;
		case 10: 
	   		echo "Please Submit an entry";
			echo '<meta http-equiv="refresh" content="3;URL=management/studentmgmt.php">';
		   break;  
		case 11: 
	   		echo "No such a record...";
			echo '<meta http-equiv="refresh" content="3;URL=management/studentmgmt.php">';
		   break; 
		case 12: 
	   		echo "Password Confirmation Fails...";
			echo '<meta http-equiv="refresh" content="3;URL=admin/adduser.php">';
		   break;
		case 13: 
	   		echo "This Username is already being Used";
			echo '<meta http-equiv="refresh" content="3;URL=admin/adduser.php">';
		   break;
		case 14: 
	   		echo "Please Submit an entry";
			echo '<meta http-equiv="refresh" content="3;URL=admin/userop.php">';
		   break;  
		case 15: 
	   		echo "No such a record...";
			echo '<meta http-equiv="refresh" content="3;URL=madmin/userop.php">';
		   break; */
	}
	
	

?>

