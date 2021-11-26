<?php
// inputs are $forum_id, $select
if ( phpversion() >= "4.2.0")
   	{
		extract($_POST);
   		extract($_GET);
   		extract($_SERVER);
	   	extract($_FILES);
   		extract($_ENV);
   		extract($_COOKIE);
   	}
	
include "config.inc.php";
include "functions.inc";

if ($Action=="new")
{
// inputs to this are
// $Subject
// $AuthorName
// $AuthorEmail
// $Text
// $Parent (if this is a reply)

	include "config.inc.php";

	$dbcon = mysql_connect ('localhost', $un, $pw);
	mysql_select_db ( $db );

	$indent = 1;
	$newposition = "999999";
	if (isset($Parent))	// item is a child item
	{
		$sqlparent = "SELECT Indent, Position FROM cparticle WHERE ID = ".$Parent;
		$resparent = mysql_query($sqlparent);
		$rowparent = mysql_fetch_object($resparent);

		$sqlallchildren = "SELECT * FROM cparticle WHERE ParentID = ".$Parent." ORDER BY Date DESC";
		$resallchildren = mysql_query($sqlallchildren);
		$rowallchildren = mysql_fetch_object($resallchildren);

		$indent = $rowparent->Indent+1;

		// if this one has other children
		if ($rowallchildren)
		{
			$lastposition = substr($rowallchildren->Position, -6);

			$newposition = $lastposition + 1;
			while (strlen($newposition) < 6)
				$newposition = "0".$newposition;

			// if there are more than 999999 childs
			// if x > 999999 then x = 999999

			$newposition = $rowparent->Position.".".$newposition;
		}
		else // if this has NO children
		{
			$newposition = $rowparent->Position.".000001";
		}
	}
	else	// item is a parent item (new thread)
	{
		// find the minimum position and decrement by 1
		$sqlpos = "SELECT Position FROM cparticle ORDER BY Position";
		$respos = mysql_query($sqlpos);
		if ($respos)
			if ($row = mysql_fetch_object($respos))
				$newposition = substr($row->Position,0,6)-1;

	}
	if (isset($Parent)==false)
		$Parent = 0;
	if (isset($forum_id)==false)
		$forum_id = 0;
	// insert the record
	$sqlinsert = "INSERT INTO cparticle (ParentID,ForumID,Subject,AuthorName,AuthorEmail,Date,Text,Notify,Position,Indent) VALUES (".
			$Parent.", ".$forum_id.", \"".$Subject."\", \"".$AuthorName."\", \"".$AuthorEmail."\", NOW(), \"".$Text."\", \"\", \"".$newposition."\", ".$indent.")";
	$resinsert = mysql_query($sqlinsert);
}

if (isset($forum_id))
{ } else { $forum_id = 0; }

?><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">

<html>
<head>
	<title>CP PHP Forum</title>

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

<p>
<?php echo $FONT4; ?><br>CP PHP Forum</font><BR>
<?php echo $FONT2; ?><br>
forum_id defaults to 0 if not given<BR>
remember to set up the MySql database <a href="install.txt">here</a><BR>
setup the config.inc file with the database name, user name and password <a href="config.inc">here</a><BR>
this is a prototype - for updates see <a href="http://www.brando.com/forum/">http://www.brando.com/forum/</a>, good luck!<BR>
</font><BR>
</p>
<BR>

<p><?php echo $FONT1; ?>[<a href="<?php getUriNew($forum_id); ?>" target="_top">New Message</a>]</font></p>


<table border="0" cellspacing="0" cellpadding="0" width="100%">
	<tr>
		<td width="70%" bgcolor="<?php echo $COLOR_HEADER_BG; ?>">
			<table border="0" cellspacing="0" cellpadding="2" width="100%">
				<tr>
					<td><?php echo $FONT1; ?>Message&nbsp;&nbsp;</font></td>
				</tr>
			</table>
		</td>
		<td width="20%" bgcolor="<?php echo $COLOR_HEADER_BG; ?>">
			<table border="0" cellspacing="0" cellpadding="0" width="100%">
				<tr>
					<td><img src="img_forum/t.gif" border="0" width="1" height="1"></td>
				</tr>
				<tr>
					<td><?php echo $FONT1; ?>Author&nbsp;&nbsp;</font></td>
				</tr>
				<tr>
					<td><img src="img_forum/t.gif" border="0" width="1" height="1"></td>
				</tr>
			</table>
		</td>
		<td width="10%" bgcolor="<?php echo $COLOR_HEADER_BG; ?>">
			<table border="0" cellspacing="0" cellpadding="0" width="100%">
				<tr>
					<td><img src="img_forum/t.gif" border="0" width="1" height="1"></td>
				</tr>
				<tr>
					<td><?php echo $FONT1; ?>Date&nbsp;&nbsp;</font></td>
				</tr>
				<tr>
					<td><img src="img_forum/t.gif" border="0" width="1" height="1"></td>
				</tr>
		</table>
		</td>
	</tr>
	<tr><td colspan="3"><img src="img_forum/t.gif" border="0" width="1" height="2"></td></tr>


<?php

	$dbcon = mysql_connect ('localhost', $un, $pw);
	mysql_select_db ( $db );
	$sql= "SELECT * FROM cparticle WHERE ForumID = ".$forum_id." ORDER BY Position";
	$res = mysql_query($sql);
	if ($res)
	while ($row = mysql_fetch_object($res))
	{
		// store some decision values.
		$parent = $row->ParentID; // whether the current article is a parent article
		$selected = 0;
		// whether the current article is selected
		if ($row->ID == $select)
			$selected = 1;
		// design: all parent entries are bold
		$bold_in = "";
		$bold_out = "";
		if ($parent==0)
		{
			$bold_in  = "<b>";
			$bold_out = "</b>";
		}
		// design: a selected article gets other colors than an unselected article.
		$text_color    = "color =\"#000000\"";
		$select_color  = "bgcolor=\"".$COLOR_NONSELECTED_BG."\"";
		$content_color = "";
		if ($selected)
		{
			$text_color    = "color  =\"".$COLOR_SELECTED_FG."\"";
			$select_color  = "bgcolor=\"".$COLOR_SELECTED_BG."\"";
			$content_color = "bgcolor=\"".$COLOR_CONTENT_BG."\"";
		}
		$cnt_txt = "";
		// TODO if the forum is in admin mode, there are serveral
		// links that allows you to edit and remove articles.
		// $admin_links = "";
		// if fh.isInAdminMode then
		// 	admin_links = ""
		// 	admin_links = admin_links & FONT1 &"<b><em>"
		// 	admin_links = admin_links & "&nbsp;&nbsp;&nbsp;&nbsp; "
		// 	admin_links = admin_links & "<a title=""Modify this message"" href=""" &fh.getUriModifyCurMsg(rs_entries("ID"))& """ target=""_top"">"
		// 	admin_links = admin_links & "<img src=""img_forum/admin_modify.gif"" border=""0"" align=""absmiddle"">"
		// 	admin_links = admin_links & "</a>"
		// 	admin_links = admin_links & " "
		// 	admin_links = admin_links & "<a title=""Delete this message and all its children"" href=""" &fh.getUriDeleteCurMsgSubs(rs_entries("ID"))& """ target=""_top"">"
		// 	admin_links = admin_links & "<img src=""img_forum/admin_delall.gif"" border=""0"" align=""absmiddle"">"
		// 	admin_links = admin_links & "</a>"
		// 	admin_links = admin_links & "</em></b></font>"
		// else
			$admin_links = "";

		// specify the distance of two articles
		// by modifing the "height" attribute -->
		?>

		<tr><td colspan="3"><img src="img_forum/t.gif" border="0" width="1" height="<?php echo $ARTICLE_SEPERATION; ?>"></td></tr>

		<?php

	// a selected entry
		if ($selected)
		{ ?>
			<tr>
				<!-- col 1 -->
				<td width="70%">
					<table border="0" cellspacing="0" cellpadding="0" width="100%">
						<tr>
							<td><a name="<?php echo $row->ID; ?>"></a><?php echo $FONT1; WriteIndent($row->Indent, ""); ?></font></td><?php echo "<td "; echo $select_color; echo " width=\"100%\">";
							echo $FONT1;
							echo $bold_in;
							echo $row->Subject;
							echo $bold_out;
							echo $cnt_txt; echo $admin_links; ?>&nbsp;&nbsp;</font></td>
						</tr>
					</table>
				</td>
				<!-- col 2 -->
				<td nowrap <?php echo $select_color; ?> width="20%"><?php echo $FONT1; echo $bold_in; echo $row->AuthorName; echo $bold_out; ?>&nbsp;&nbsp;</font></td>
				<!-- col 3 -->
				<td nowrap <?php echo $select_color; ?> width="10%"><? echo $FONT1; echo $bold_in;
				echo $row->Date;
				//FormatDateTime(rs_entries("Date"),vbShortDate)
				// TODO ABOVE
				echo $bold_out; ?>&nbsp;&nbsp;</font></td>
			</tr>
<?php } ?>

			<!-- the content of a selected entry -->
			<?php if ($select==$row->ID)
			{ ?>
				<tr>
					<!-- col 1,2 and 3 -->
					<td colspan="3" width="100%"><table border="0" cellspacing="0" cellpadding="0" width="100%">
						<tr>
							<td><?php echo $FONT1; WriteIndent($row->Indent, "content"); ?></font></td>
							<td <?php echo $content_color; ?> width="100%"><table border="0" cellspacing="5" cellpadding="0" width="100%">
								<tr>
									<td><?php echo $FONT1; echo fmtContent($row->Text); ?><br>&nbsp;<br>
										[<?php echo "<a	href=\""; getUriReplyForum($row->ForumID, $row->ID); echo "\" target=\"_top\">"; echo $FONT1; ?>Reply</font></a>]
										[<a href="" target="_top"><?php echo $FONT1; ?>Email Author (not done)</font></a>]
										[<?php echo "<a	href=\""; getUriNew($row->ForumID); echo "\" target=\"_top\">"; echo $FONT1; ?>New Message</font></a>]</font></td>
								</tr>
							</table></td>
							</tr>
					</table></td>
				</tr>
			<?php  } else { ?>

		<!-- a not-selected entry -->
			<tr>
				<!-- col 1 -->
				<td width="70%"><table border="0" cellspacing="0" cellpadding="0" width="100%">
					<tr>
						<td><?php echo "<a name=\""; echo $row->ID; echo "\">"; ?></a><?php echo $FONT1; WriteIndent($row->Indent, ""); ?></font></td>
						<?php echo "<td "; echo $select_color; echo "width=\"100%\">"; echo " <a class=\"LIST\" href=\""; echo getUriSelectMsg($row->ID); echo "\" target=\"_top\">"; echo $FONT1; echo $bold_in; echo $row->Subject; echo $bold_out; ?></a>
							<?php echo $cnt_txt; echo $admin_links; ?>&nbsp;&nbsp;</font></td>
					</tr>
				</table></td>

				<!-- col 2 -->
				<td nowrap <?php echo $select_color; ?> width="20%"><?php echo $FONT1; echo $bold_in; echo $row->AuthorName; echo $bold_out; ?>&nbsp;&nbsp;</font></td>

				<!-- col 3 -->
				<td nowrap <?php echo $select_color; ?> width="10%"><?php echo $FONT1; echo $bold_in; echo $row->Date; /*TODO the last was a FmtDataTime*/ echo $bold_out; ?>&nbsp;&nbsp;</font></td>
			</tr>
		<?php } ?>
<?php  } ?>

</table>
</body>
</html>


