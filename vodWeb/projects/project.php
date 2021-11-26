<style>
<!--
#foldheader{cursor:hand ; font-weight:bold ;
list-style-image:  url(images/topic.gif)}
#foldtext{cursor:hand ; font-weight:normal ;
list-style-image:  url(images/topic.gif)}

#foldinglist{list-style-image:  url(images/list.gif)}
//-->
</style>
<script language="JavaScript1.2">
<!--

var head="display:''"
img1=new Image()
img1.src="images/fold.gif"
img2=new Image()
img2.src="images/open.gif"

function change(){
   if(!document.all)
      return
   if (event.srcElement.id=="foldheader") {
      var srcIndex = event.srcElement.sourceIndex
      var nested = document.all[srcIndex+1]
      if (nested.style.display=="none") {
         nested.style.display=''
         event.srcElement.style.listStyleImage="url(images/open.gif)"
      }
	 
      else {
         nested.style.display="none"
         event.srcElement.style.listStyleImage="url(images/fold.gif)"
      }
	  
   }
    else if (event.srcElement.id=="foldtext") {
      var srcIndex = event.srcElement.sourceIndex
      var nested = document.all[srcIndex+1]
      if (nested.style.display=="none") {
         nested.style.display=''
         event.srcElement.style.listStyleImage="url(images/open.gif)"
      } else {
         nested.style.display="none"
         event.srcElement.style.listStyleImage="url(images/fold.gif)"
      }}
}

document.onclick=change

//-->
</script>
<?php

$link=DBconnect();

$result = mysql_query("SELECT project.name AS projectname,
faculty.name AS facultyname,department.name AS departmentname,
course.name AS coursename,chapter.name AS chaptername,
project.subject AS subject,project.projectdesc AS projectdescription,
project.recorddate AS projectdate
	FROM faculty
	JOIN department
	JOIN course
	JOIN chapter
	JOIN project
WHERE (faculty.id=department.facid) AND (department.id=course.depid) AND 
(course.id=chapter.csid) AND (chapter.id=project.chid)
AND (project.id='$prid')", $link);

if (!$result) die("query failed: ".mysql_error());
$row = mysql_fetch_array($result);
?>
<table width="640" border="0" align="center" cellspacing="0">
  <tr>
    <td bgcolor="#996600"><div align="center"><font color="#FFCC00" size="6"><strong>Project Information</strong></font></div></td>
  </tr>
</table>
<br>
<table width="640" border="1" align="center" cellspacing="0">
  <tr bgcolor="#996600"> 
    <td colspan="2"><strong><font color="#FFCC00" size="5">General Information:</font></strong></td>
  </tr>
  <tr> 
    <td width="146" bgcolor="#996600"><font color="#FFCC00" size="3"><strong>Project Name:</strong></font></td>
    <td width="484" bgcolor="#FFCC00"><? echo $row["projectname"]; ?></td>
  </tr>
  <tr> 
    <td bgcolor="#996600"><font color="#FFCC00" size="3"><strong>Faculty:</strong></font></td>
    <td bgcolor="#FFCC00"><? echo $row["facultyname"]; ?></td>
  </tr>
  <tr> 
    <td bgcolor="#996600"><font color="#FFCC00" size="3"><strong>Department:</strong></font></td>
    <td bgcolor="#FFCC00"><? echo $row["departmentname"]; ?></td>
  </tr>
  <tr> 
    <td bgcolor="#996600"><font color="#FFCC00" size="3"><strong>Course:</strong></font></td>
    <td bgcolor="#FFCC00"><? echo $row["coursename"]; ?></td>
  </tr>
  <tr> 
    <td bgcolor="#996600"><font color="#FFCC00" size="3"><strong>Chapter:</strong></font></td>
    <td bgcolor="#FFCC00"><? echo $row["chaptername"]; ?></td>
  </tr>
</table>
<br>
<table width="640" border="1" align="center" cellspacing="0">
  <tr bgcolor="#996600"> 
    <td colspan="3"><strong><font color="#FFCC00" size="5">Specific Information:</font></strong></td>
  </tr>
  <tr> 
    <td width="212" bgcolor="#996600"><font color="#FFCC00" size="3"><strong>Subject:</strong></font></td>
    <td colspan="2" bgcolor="#FFCC00"><? echo $row["subject"]; ?></td>
  </tr>
  <tr> 
    <td bgcolor="#996600"><font color="#FFCC00" size="3"><strong>Project Description:</strong></font></td>
    <td colspan="2" bgcolor="#FFCC00"><? echo $row["projectdescription"]; ?></td>
  </tr>
  <tr> 
    <td bgcolor="#996600"><font color="#FFCC00" size="3"><strong>Owner:</strong></font></td>
    <td colspan="2" bgcolor="#FFCC00"><? echo $row["usertitle"],' ',$row["username"],' ',$row["usersurname"]; ?></td>
  </tr>
</table>
  

 
  

<table width="100%"  border="1" cellspacing="0" cellpadding="0">
  <tr>
    <th scope="col"><font color="#FFCC00"><li id="foldheader">Discussion Threads</li>
<ul id="foldinglist" style="display:none" style=&{head};>
    </th>
  </tr>
  <tr>
    <td bgcolor="#FFCC00"><a href="forum.php?fa=new">Add new topic</a></td>
  </tr> 
  <?
 $result = mysql_query("SELECT * FROM forumtitle where prid=$prid order by ftime DESC", $link);
	if (!$result) die("query failed: ".mysql_error());
	while($row = mysql_fetch_array($result)){?>   </tr><tr> <td ><?
	echo'<li id="foldheader">',$row["title"],'</li> <ul id="foldinglist" style="display:none" style=&{head};>';
	$id = $row["titid"];
		$result1 = mysql_query("SELECT * FROM forum where titleid=$id order by ftime ASC", $link);
		if (!$result1) die("query failed: ".mysql_error());
		while($row1 = mysql_fetch_array($result1)){
echo'<li id="foldtext">',$row1["ttitle"],'</li> <ul id="foldinglist" style="display:none" style=&{head};>';
		
		echo $row1["ftext"];
//	echo '</ul>';

echo '</ul>';
}
?></td></tr> <? echo '</ul>';
}

echo '</ul>';
?>	
</table>
