<?php
// inputs are $forum_id, $select

include "config.inc.php";

?><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">

<html>
<head>
	<title>CP PHP Forum - new</title>

<style type="text/css">
	.nav:link    { text-decoration:none; color:#000000; }
	.nav:visited { text-decoration:none; color:#000000; }
	.nav:active  { text-decoration:none; color:#000000; }
	.nav:hover   { text-decoration:underline; color:#0000a0; }
	.nav2:link    { text-decoration:none; color:#000000; }
	.nav2:visited { text-decoration:none; color:#000000; }
	.nav2:active  { text-decoration:none; color:#000000; }
	.nav2:hover   { text-decoration:underline; color:#0000a0; }
</style>
<link rel="stylesheet" type=text/css href="css/global.css">
</head>

<body>

		<script language="JavaScript"><!--
			function chkFormular() {
				if ( document.form.Subject.value=="" ) {
					alert( "Please enter a subject" );
					document.form.Subject.focus();
					return false;
				}

				if ( document.form.Text.value=="" ) {
					alert( "Please enter a message" );
					document.form.Text.focus();
					return false;
				}

				if ( document.form.AuthorName.value=="" ) {
					alert( "Please enter your name" );
					document.form.AuthorName.focus();
					return false;
				}

				if ( document.form.AuthorEmail.value=="" ) {
					alert( "Please enter your e-mail address" );
					document.form.AuthorEmail.focus();
					return false;
				}
			}
		//--></script>

		<form action="index.php?Action=new&forum_id=<?php echo $forum_id; ?>" method="post"
			name="form" onsubmit="return chkFormular()">

			<input type="hidden" name="forumid"  value="<%=forum_id%>">

			<div align="left">
				<table border="0" cellspacing="0">
					<tr>
						<td valign="middle"><font size="1" face="Verdana">Subject:</font></td>
						<td valign="middle" colspan="2"><font size="1"
							face="Verdana"><input type="text" size="35"
							name="Subject"></font></td>
					</tr>
					<tr>
						<td valign="top"><font size="1" face="Verdana">Text:</font></td>
						<td valign="top" colspan="2"><font size="1"
							face="Verdana"><textarea name="Text" rows="15"
							cols="60"></textarea></font></td>
					</tr>
					<tr>
						<td valign="top">&nbsp;</td>
						<td valign="top" colspan="2">&nbsp;</td>
					</tr>
					<tr>
						<td valign="middle"><font size="1" face="Verdana">Your Name:</font></td>
						<td valign="middle" colspan="2"><font size="1"
							face="Verdana"><input type="text" size="35"
							name="AuthorName" value="<?php echo $AuthorName; ?>"></font></td>
					</tr>
					<tr>
						<td valign="middle"><font size="1" face="Verdana">E-Mail:</font></td>
						<td valign="middle" colspan="2"><font size="1"
							face="Verdana"><input type="text" size="35"
							name="AuthorEmail" value=""></font></td>
					</tr>
					<tr>
						<td valign="middle"><font size="1" face="Verdana">&nbsp;</font></td>
						<td valign="middle" colspan="2"><font size="1"
							face="Verdana"><input type="checkbox" name="email_notify"
							value="ja" checked> Notify me by e-mail if someone
							answers to this message</font></td>
					</tr>
					<tr>
						<td valign="top"><font face="verdana" size="1">&nbsp;</font></td>
						<td valign="top" colspan="2"><font face="verdana" size="1">&nbsp;</font></td>
					</tr>
					<tr>
						<td valign="middle"><font size="1" face="Verdana">Password:</font></td>
						<td valign="middle" colspan="2"><font size="1"
							face="Verdana"><input type="text" size="40"
							name="ArticlePassword" value=""> Set only if you want to 
							modify your message later.</font></td>
					</tr>
					<tr>
						<td valign="top">&nbsp;</td>
						<td valign="top" colspan="2">&nbsp;</td>
					</tr>
					<tr>
						<td valign="top">&nbsp;</td>
						<td valign="top" colspan="2"><font size="1"
							face="Verdana"><input type="submit" name="B1"
							value="Submit"></font></td>
					</tr>
				</table>
			</div>
		</form>
</body>
</html>
