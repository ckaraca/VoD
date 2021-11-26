<?php
include("../libs/vodlib.php");
	
	if(isset($hash)) 
	{//There is Cookie
		if(CheckHash($hash,$user)>0) //if the hash information is correct
		{
			header("location:vod.php?action=menu.php");
		}else{
			header("location:login.php");
		}
		
	}else{ //There is NO Cookie
		header("location:login.php");
	}


?>
<html>
<head>
<title>Video on Demand</title>
<meta http-equiv="Content-Type" content="text/html; charset=">
</head>
<body>
<?php
	

?>
</body>
</html>
