
<table width="550" border="0" align="center" cellspacing="0">
  <tr> 
    <td width="563" bgcolor="#FFFFCC"><div align="center"><font color="#990000" size="+7"><strong>New 
        User Information</strong></font></div></td>
  </tr>
</table>
<form name="form1" method="post" action="vod.php?action=adm/saveuser.php&op=1">
  <table width="550" border="1" align="center" cellspacing="0">
    <tr bgcolor="#FFFFCC"> 
      <td colspan="2"><strong><font color="#993300" size="4">Personal Information 
        : </font></strong></td>
    </tr>
    <tr> 
      <td width="196" bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>Title 
          : </strong></font></div></td>
      <td width="350"><input name="title" type="text" id="title"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>Name 
          : </strong></font></div></td>
      <td><input name="name" type="text" id="name"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>Surname 
          : </strong></font></div></td>
      <td><input name="surname" type="text" id="surname"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><font color="#996600"><strong>E-mail :</strong></font></td>
      <td><input name="mail" type="text" id="mail"></td>
    </tr>
  </table>
  <br>
  <table width="550" border="1" align="center" cellspacing="0">
    <tr bgcolor="#FFFFCC"> 
      <td colspan="2"><strong><font color="#993300" size="4">Login Information 
        : </font></strong></td>
    </tr>
    <tr> 
      <td width="196" bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>Login 
          name :</strong></font></div></td>
      <td width="350"><input name="user" type="text" id="user"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>Password 
          : </strong></font></div></td>
      <td><input name="pass1" type="password" id="pass1"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>Confirm 
          Password :</strong></font></div></td>
      <td><input name="pass2" type="password" id="pass2"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><font color="#996600"><strong>Privilege :</strong></font></td>
      <td><select name="privilege" id="privilege">
         <option value="0" selected>Student</option>
          <option value="1" >Instructor</option>
          <option value="2">Administrator</option>
        </select></td>
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
    </table></div>
</form>