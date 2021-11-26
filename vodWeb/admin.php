<?php include("../libs/vod.php"); Authorize($hash,$user)?>
<html>
<head>
<title>Untitled Document</title>
<meta http-equiv="Content-Type" content="text/html; charset=">
</head>

<body>

<?php
	$DB=DBparameters();
	$link=mysql_connect($DB["host"],$DB["username"],$DB["password"]) or die ("Could't Connect to the Server");
	mysql_select_db ($DB["dbname"])or die ("The Database cannot be selected");
	$result = mysql_query("SELECT * FROM system", $link);
	if (!$result) die("query failed: ".mysql_error());
	$row = mysql_fetch_array($result);

echo'
<table width="550" border="0" align="center" cellspacing="0">
  <tr> 
    <td width="563" bgcolor="#FFFFCC"><div align="center"><font color="#990000" size="+7"><strong>VoD Administration Page </strong></font></div></td>
  </tr>
</table>
<form name="form1" method="post" action="">
  <table width="550" border="1" align="center" cellspacing="0">
    <tr bgcolor="#FFFFCC"> 
      <td colspan="2"><strong><font color="#993300" size="4">Login Options:</font></strong></td>
    </tr>
    <tr> 
      <td width="196" bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>Name of the Cookie :</strong></font></div></td>
      <td width="350"><input name="cookiename" type="text" id="cookiename" value="',$row["cookiename"],'"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>Expiration Time :</strong></font></div></td>
      <td><input name="exptime" type="text" id="exptime" value="',$row["exptime"],'"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>Md5 Secret Key</strong></font></div></td>
      <td><input name="hkey" type="text" id="hkey" value="',$row["hkey"],'"></td>
    </tr>
  </table>
  <br>
  <table width="550" border="1" align="center" cellspacing="0">
    <tr bgcolor="#FFFFCC"> 
      <td colspan="2"><strong><font color="#993300" size="4">Database Options:</font></strong></td>
    </tr>
    <tr> 
      <td width="231" bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>MySQL Server : </strong></font></div></td>
      <td width="405"><input name="myserver" type="text" id="myserver" value="',$row["myserver"],'"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>MySQL User :</strong></font></div></td>
      <td><input name="myuser" type="text" id="myuser" value="',$row["myuser"],'"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>MySQL Password :</strong></font></div></td>
      <td><input name="mypass" type="password" id="mypass" value="',$row["mypass"],'"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>MySQL Database :</strong></font></div></td>
      <td><input name="mydatabase" type="text" id="mydatabase" value="',$row["mydatabase"],'"></td>
    </tr>
  </table>
  <br>
  <table width="550" border="1" align="center" cellspacing="0">
    <tr bgcolor="#FFFFCC"> 
      <td colspan="2"><strong><font color="#993300" size="4">Contact Options:</font></strong></td>
    </tr>
    <tr> 
      <td width="234" bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>Contact Email :</strong></font></div></td>
      <td width="402"><input name="cemail" type="text" id="cemail" value="',$row["cemail"],'"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><font color="#996600"><strong>Error Email :</strong></font></td>
      <td><input name="eemail" type="text" id="eemail" value="',$row["eemail"],'"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><font color="#996600"><strong>FAQ Email :</strong></font></td>
      <td><input name="faqemail" type="text" id="faqemail" value="',$row["faqemail"],'"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><strong><font color="#996600">Admin Name and Surname :</font></strong></td>
      <td><input name="adminname" type="text" id="adminname" value="',$row["adminname"],'"></td>
    </tr>
  </table>
  <br>
  <div align="center">
    <table width="550" border="0" align="center" cellspacing="0">
      <tr> 
        <td width="563" bgcolor="#FFFFCC"><div align="center">
            <input type="submit" name="Submit" value="Save">
          </div></td>
      </tr>
    </table>';
	
?>
    
  </div>
</form>
<br>
</body>
</html>
