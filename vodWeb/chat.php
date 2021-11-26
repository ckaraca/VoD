<?PHP

if ( phpversion() >= "4.2.0")
   	{
		extract($_POST);
   		extract($_GET);
   		extract($_SERVER);
	   	extract($_FILES);
   		extract($_ENV);
   		extract($_COOKIE);
   	}
   	
function ShowLoginForm()
{ 
	?>
	<b>Enter Your NickName</b> 
	<form name="chat" method="post" action="chat.php" target="_top"> 
	<input type="text" name="nick" size="20"> 
	<input type="hidden" name="action" value="enter"> 
	<input type="hidden" name="chat" value="<font color=FF0000><b>Enters the Room</b></font>"> 
	<input type="submit" name="Submit" value="Submit"> 
	</form>
	<?php 
} 

function Login()
{
	
	global $chat; 
	global $nick; 
	session_start(); 
	session_register("nick", $nick); 
	?>
	<frameset cols="230,*" rows="*" framespacing="0" bordercolor="#CCCCCC"> 
	<frame name="form" scrolling="no"  marginwidth="0" marginheight="0" noresize src="chat.php?action=form&nick=<?php echo $nick; ?>"> 
	<frame name="posts"   src="chat.php?action=posts&nick=<?php echo $nick; ?>&chat=<?php echo $chat; ?>"> 
	</frameset>
	<noframes> 
	<body>
	<p>This page uses frames, but your browser doesn't support them.</p>
	</body> 
	</noframes> 
	</frameset>
	<?php 
}

function GetInput()
{
	global $HTTP_SESSION_VARS; 
	global $chat; 
	global $nick;
	?> 
	<form onSubmit="return doSubmit" name="chatform" method="post" action="chat.php" target="posts">
<table width="100%"  border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td colspan="5"><input type="text" name="chat" size="26">      </td>
  </tr>
  <tr>
    <td><input type="button" onClick="doSubmit()" name="Submit" value="Send"></td>
    <td><select name="col">
      <option>Black</option>
      <option>Red</option>
      <option>Green</option>
      <option>Blue</option>
      <option>Orange</option>
    </select></td>
    <td><select name="font">
  <option value="normal">Normal</option>
  <option value="strong">Bold</option>
  <option value="em">Italic</option>
  <option value="u">Underline</option>
</select></td>
    <td><input type="hidden" name="nick" value="<?php echo $nick; ?>"></td>
    <td><input type="hidden" name="action" value="posts"></td>
  </tr>
</table>
</form>
	<script language="JavaScript">
	
	function doSubmit() 
	{
		if(document.chatform.chat.value == '')
		{ 
			document.chatform.chat.focus(); 
			return false;
		}
		document.chatform.chat.value = '<font color="'+document.chatform.col[document.chatform.col.selectedIndex].text+'"><'+document.chatform.font[document.chatform.font.selectedIndex].value+'>'+document.chatform.chat.value+'</'+document.chatform.font[document.chatform.font.selectedIndex].value+'></font>';
		document.chatform.submit(); 
		document.chatform.chat.value = ''; 
		document.chatform.chat.value = ''; 
		document.chatform.chat.focus();
		return true;
	}
	</script>
	<?php 

}

function ShowAddPosts()
{
	global $HTTP_SESSION_VARS;
	global $chat;
	global $nick;	
	print '<meta http-equiv="refresh" content="5;URL=chat.php?action=posts&nick=<? echo $nick; ?>">';
	$svrConn = mysql_connect("localhost", "cem", "ankara") or die("<b>Error:</b> Couldnt connect to database");
	$dbConn = mysql_select_db("vod", $svrConn) or die ("<b>Error:</b> Couldnt connect to database");
	if(!empty($chat))
	{ 
		$strQuery = "insert into chatText  values('', '$chat', '$nick',NOW())"; 
		$sql = "insert into chatuser values('', '$nick', NOW())";
		$result =mysql_query($strQuery); 
		$result =mysql_query($sql); 
		if (!$result)
		{
			$sql ="UPDATE chatuser SET ttime = NOW( ) WHERE nick=\"$nick\" LIMIT 1";
			$result =mysql_query($sql); 
		}
	}
	$strQuery = "select * from chatText order by id desc limit 20"; 
	$chats = mysql_query($strQuery);
	
	$delete = "delete from chatText where ttime<(NOW()-20000);"; 
	$delete = "delete from chatuser where ttime<(NOW()-1000);";
	mysql_query($delete);
	mysql_close($svrConn);
	function swapFaces($chatLine)
	{
		
		return $chatLine;
	}

	while($chatline = mysql_fetch_array($chats)) 
	{ 
		print "<b>" . $chatline["nick"] . ":</b> " . swapFaces($chatline["text"]) . "<br>";
	}
	
}
if (empty($action)) 
ShowLoginForm(); 

elseif ($action == "posts") 
ShowAddPosts(); 

elseif ($action == "form") 
GetInput(); 

elseif ($action == "enter") 
Login();

?>