<?php

	include ("../libs/vodlib.php");
	if(!empty($_POST["loginname"]))
	{
		if ($userid=Login($_POST["loginname"],$_POST["password"])==-1)
		{ //the user id is -1
			header("location:error.php?errno=1");
		}else{ //the userid is exist
			SendCookie($_POST["loginname"]);
			header("location:vod.php?action=menu.php");
		}
	}else{
	
?>
<html>
<head>
<title>Video on Demand</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-9">
<link href="vod.css" rel="stylesheet" type="text/css">
</head>

<body bgcolor="#6766CC" leftmargin="0" topmargin="0" marginwidth="0" marginheight="0">
<form name="form1" method="post" action="login.php">
<TABLE WIDTH=800 BORDER=0 align="center" CELLPADDING=0 CELLSPACING=0>
	<TR>
		<TD COLSPAN=3>
			<IMG SRC="images/vod1_01.jpg" WIDTH=800 HEIGHT=222 ALT=""></TD>
	</TR>
	<TR>
		<TD ROWSPAN=2>			<img src="images/vod1_02.jpg" width=289 height=274 alt=""></TD>
		<TD width="222" height="156" background="images/vod1_03.jpg">
		
		<table width="100%"  border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td colspan="2"><div align="center"><strong><font color="#FFFFFF" face="Tahoma">Login</font></strong></div></td>
          </tr>
          <tr>
            <td><strong><font color="#FFFFFF" face="Tahoma">Name</font></strong></td>
            <td><input name="loginname" type="text" id="loginname3"></td>
          </tr>
          <tr>
            <td><strong><font color="#FFFFFF" face="Tahoma">Pass</font></strong></td>
            <td><input name="password" type="password" id="password4"></td>
          </tr>
          <tr>
            <td colspan="2"><div align="center">
              <input type="submit" name="Submit" value="Submit">
            </div></td>
          </tr>
        </table></TD>
		<TD ROWSPAN=2>
			<IMG SRC="images/vod1_04.jpg" WIDTH=289 HEIGHT=274 ALT=""></TD>
	</TR>
	<TR>
		<TD>
			<IMG SRC="images/vod1_05.jpg" WIDTH=222 HEIGHT=118 ALT=""></TD>
	</TR>
</TABLE>

</form>
<?PHP
}
?>
<div align="center"><font color="#FFFFFF" size="-1" face="Tahoma">Copyright © 2002 - 2003 Cem KARACA<br>
    <a href="http://interdec.emu.edu.tr" class="vod">Designed at Information Technologies R&amp;D Center</a>,<br>
    <a href="http://www.emu.edu.tr" class="vod">Eastern Mediterranean University</a></font></div>
</body>
</html>

