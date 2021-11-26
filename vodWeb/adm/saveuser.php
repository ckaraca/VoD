<?php
	if(!isset($hash)) header("location:error.php?errno=3");
	$link=DBconnect();
	
	
	if($_GET["op"]==1){
		
		$result = mysql_query("SELECT userid FROM user WHERE user.user='$_POST[user]'", $link);
		if (!$result) die("query failed: ".mysql_error());
		$row = mysql_fetch_array($result);
		
		if(!$row) 
		{
			if($_POST["pass1"]!=$_POST["pass2"]) echo '<meta http-equiv="refresh" content="0;URL=error.php?errno=4">';
			$result = mysql_query("INSERT INTO user (user,pass,priv,title,name,surname,mail) VALUES ('$_POST[user]',PASSWORD('$_POST[pass1]'),'$_POST[priv]','$_POST[title]','$_POST[name]','$_POST[surname]','$_POST[mail]')",$link);
			if (!$result) die("query failed: ".mysql_error());
			DBdisconnect($link);
			echo 'Successfully Saved';
		}else{
			echo '<meta http-equiv="refresh" content="0;URL=error.php?errno=5">';
		}
	}elseif($_GET["op"]==2){
		if(empty($_POST["pass1"]))
		{
			$result = mysql_query("UPDATE user SET user='$_POST[user]',priv='$_POST[priv]',title='$_POST[title]',name='$_POST[name]',surname='$_POST[surname]',mail='$_POST[mail]' WHERE userid='$_GET[uid]'", $link);
		}else
		{
			if($_POST["pass1"]==$_POST["pass2"])
			{
				$result = mysql_query("UPDATE user SET user='$_POST[user]',pass=PASSWORD('$_POST[pass1]'),priv='$_POST[priv]',title='$_POST[title]',name='$_POST[name]',surname='$_POST[surname]',mail='$_POST[mail]' WHERE userid='$_GET[uid]'", $link);
			}else{
				//Error of 2 different passwords
			}	
		}
		if (!$result) die("query failed: ".mysql_error());
		DBdisconnect($link);
		echo 'Successfully Saved';
		
	}
	
	
?>