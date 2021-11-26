# phpMyAdmin MySQL-Dump
# version 2.4.0
# http://www.phpmyadmin.net/ (download page)
#
# Sunucu:: localhost
# Çýktý Tarihi: Aralýk 11, 2003 at 05:59 AM
# Server sürümü: 4.0.13
# PHP Sürümü: 4.3.1
# Veritabaný : `vod_web`
# --------------------------------------------------------

#
# Tablo için tablo yapýsý `chapter`
#

CREATE TABLE chapter (
  id int(1) unsigned NOT NULL auto_increment,
  name varchar(100) default NULL,
  csid int(1) unsigned NOT NULL default '0',
  chapterdesc text,
  status char(1) NOT NULL default '1',
  recorddate timestamp(12) NOT NULL,
  note text,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Tablo döküm verisi `chapter`
#

INSERT INTO chapter VALUES (14, 'Star sailor', 4, '', '1', 030706000838, NULL);
INSERT INTO chapter VALUES (12, 'Queen', 4, '', '1', 030706000838, NULL);
INSERT INTO chapter VALUES (11, 'Madonna', 4, '', '1', 030706000838, NULL);
INSERT INTO chapter VALUES (5, 'Chapter 5', 1, 'c', '1', 030706000838, NULL);
INSERT INTO chapter VALUES (6, 'ch 1', 2, 'd', '1', 030706000838, NULL);
INSERT INTO chapter VALUES (7, 'Chapter 1', 3, 'EEE Dept. Model Animations', '1', 030706000838, NULL);
INSERT INTO chapter VALUES (8, 'Twain', 4, 'e', '1', 030706000838, NULL);
INSERT INTO chapter VALUES (10, 'enya', 4, '', '1', 030706000838, NULL);
INSERT INTO chapter VALUES (13, 'Chapter 1', 5, '', '1', 030706000838, NULL);
INSERT INTO chapter VALUES (15, 'Nirvana', 6, '', '1', 030706000838, NULL);
INSERT INTO chapter VALUES (16, 'Judas', 6, '', '1', 030706000838, NULL);
INSERT INTO chapter VALUES (17, 'P.O.D', 6, '', '1', 030706000838, NULL);
INSERT INTO chapter VALUES (18, 'Yngwie Malmsteen', 6, '', '1', 030706000838, NULL);
INSERT INTO chapter VALUES (19, 'EVANESCENCE', 4, '', '1', 030706000838, NULL);
# --------------------------------------------------------

#
# Tablo için tablo yapýsý `course`
#

CREATE TABLE course (
  id int(1) unsigned NOT NULL auto_increment,
  depid int(1) unsigned NOT NULL default '0',
  cscode varchar(10) default NULL,
  name varchar(50) default NULL,
  instructor varchar(60) default NULL,
  nofst int(1) unsigned default NULL,
  coursedesc text,
  status char(1) NOT NULL default '1',
  recorddate timestamp(12) NOT NULL,
  note text,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Tablo döküm verisi `course`
#

INSERT INTO course VALUES (1, 1, 'ee 500', 'Thesis', 'Derviþ Z. Deniz', NULL, '', '1', 030706000803, NULL);
INSERT INTO course VALUES (2, 18, 'med 101', '', '', NULL, '', '1', 030706000803, NULL);
INSERT INTO course VALUES (3, 1, 'EEE 474', 'Software Engineering', 'Dervis Z. Deniz', NULL, '', '1', 030706000803, NULL);
INSERT INTO course VALUES (4, 10, 'Pop Music', 'Pop Video', '', NULL, '', '1', 030706000803, NULL);
INSERT INTO course VALUES (5, 1, 'EE 511', 'Design, Analysis and Performance of computer netwo', 'Dervis Z. Deniz', NULL, '', '1', 030706000803, NULL);
INSERT INTO course VALUES (6, 10, 'Rock Music', 'MP3', '', NULL, '', '1', 030706000803, NULL);
# --------------------------------------------------------

#
# Tablo için tablo yapýsý `department`
#

CREATE TABLE department (
  id int(1) unsigned NOT NULL auto_increment,
  facid int(1) unsigned NOT NULL default '0',
  name varchar(50) default NULL,
  status char(1) NOT NULL default '1',
  recorddate timestamp(12) NOT NULL,
  note text,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Tablo döküm verisi `department`
#

INSERT INTO department VALUES (1, 1, 'Electrical & Electronic', '1', 030706000536, NULL);
INSERT INTO department VALUES (2, 2, 'Architecture', '1', 030706000536, NULL);
INSERT INTO department VALUES (3, 2, 'Interior Architecture', '1', 030706000536, NULL);
INSERT INTO department VALUES (4, 3, 'Applied Mathematics & Computer Science', '1', 030706000536, NULL);
INSERT INTO department VALUES (5, 3, 'Archaeology & History of Art', '1', 030706000536, NULL);
INSERT INTO department VALUES (6, 3, 'Chemistry', '1', 030706000536, NULL);
INSERT INTO department VALUES (7, 3, 'English Literature and Humanities', '1', 030706000536, NULL);
INSERT INTO department VALUES (8, 3, 'History', '1', 030706000536, NULL);
INSERT INTO department VALUES (9, 3, 'Information Systems', '1', 030706000536, NULL);
INSERT INTO department VALUES (10, 3, 'Music', '1', 030706000536, NULL);
INSERT INTO department VALUES (11, 3, 'Physics', '1', 030706000536, NULL);
INSERT INTO department VALUES (12, 3, 'Turkish Language & Literature', '1', 030706000536, NULL);
INSERT INTO department VALUES (13, 4, 'Banking & Finance', '1', 030706000536, NULL);
INSERT INTO department VALUES (14, 4, 'Business Administration', '1', 030706000536, NULL);
INSERT INTO department VALUES (15, 4, 'Economics', '1', 030706000536, NULL);
INSERT INTO department VALUES (16, 4, 'International Relations', '1', 030706000536, NULL);
INSERT INTO department VALUES (17, 4, 'Political Science & Public Administration', '1', 030706000536, NULL);
INSERT INTO department VALUES (18, 8, 'Medicine', '1', 030706000536, NULL);
# --------------------------------------------------------

#
# Tablo için tablo yapýsý `faculty`
#

CREATE TABLE faculty (
  id int(1) unsigned NOT NULL auto_increment,
  name varchar(50) default NULL,
  status char(1) NOT NULL default '1',
  recorddate timestamp(12) NOT NULL,
  note text,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Tablo döküm verisi `faculty`
#

INSERT INTO faculty VALUES (1, 'Engineering', '1', 030706000357, '');
INSERT INTO faculty VALUES (2, 'Architecture', '1', 030706000357, '');
INSERT INTO faculty VALUES (3, 'Arts & Science', '1', 030706000357, '');
INSERT INTO faculty VALUES (4, 'Business & Economics', '1', 030706000357, '');
INSERT INTO faculty VALUES (5, 'Communication & Media Studies', '1', 030706000357, '');
INSERT INTO faculty VALUES (6, 'Education', '1', 030706000357, '');
INSERT INTO faculty VALUES (7, 'Law', '1', 030706000357, '');
INSERT INTO faculty VALUES (8, 'Medicine', '1', 030706000357, '');
# --------------------------------------------------------

#
# Tablo için tablo yapýsý `ftp`
#

CREATE TABLE ftp (
  id int(1) unsigned NOT NULL auto_increment,
  Host varchar(220) NOT NULL default '',
  Port tinyint(1) unsigned NOT NULL default '21',
  User varchar(20) default NULL,
  Pass varchar(20) default NULL,
  active char(1) NOT NULL default '0',
  recorddate timestamp(12) NOT NULL,
  note text,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Tablo döküm verisi `ftp`
#

INSERT INTO ftp VALUES (1, '213.208.49.37', 21, 'control', 'ankara', '0', 030706004027, NULL);
INSERT INTO ftp VALUES (2, '213.208.49.37', 21, 'VoD', 'ftp', '0', 030706004027, NULL);
# --------------------------------------------------------

#
# Tablo için tablo yapýsý `project`
#

CREATE TABLE project (
  id int(1) unsigned NOT NULL auto_increment,
  name varchar(100) default NULL,
  chid varchar(10) default NULL,
  subject varchar(200) default NULL,
  projectdesc text,
  VideoPath varchar(255) default NULL,
  VideoName varchar(255) default NULL,
  userid int(1) default NULL,
  status char(1) NOT NULL default '1',
  recorddate timestamp(12) NOT NULL,
  note text,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Tablo döküm verisi `project`
#

INSERT INTO project VALUES (54, 'Fool misguided fool', '14', 'Music', 'Super sarki walla\r\n', '{5A77D9F2-9899-46ab-BC77-DF1D010C3E94}', 'starsailor.mpg', 8, '1', 030722203654, NULL);
INSERT INTO project VALUES (53, 'Warriors of the Net', '13', 'Network animation', '', '{4E4D498B-1EDB-43d8-B52C-E66BF859B965}', 'warriors-1150.mpg', 8, '1', 030722202519, NULL);
INSERT INTO project VALUES (50, 'IMAX', '7', '', '', '{C3BA4561-5E6C-4254-93ED-51056F812B60}', 'IMAX - Mysteries Of Egypt.avi', 8, '1', 030722202519, NULL);
INSERT INTO project VALUES (51, 'IMAX 1', '7', 'egypt', '', '{05183B9C-99E3-416b-9E1A-DA691581C6AD}', 'IMAX - Mysteries Of Egypt.avi', 8, '1', 030722202519, NULL);
INSERT INTO project VALUES (47, 'Guns in roses', '5', 'November rain', '', '{B431D550-EEE3-4a1a-ABA9-6E05B8C4C16E}', 'Guns n Roses - November Rain (3).mpg', 8, '1', 030722202520, NULL);
INSERT INTO project VALUES (34, 'Travis', '5', 'tttt', '', '{B95AC647-A862-4f11-9BDC-DD220BAF1E41}', 'Travis - Sing - XF.mpg', 8, '1', 030722202520, NULL);
INSERT INTO project VALUES (46, 'Shania 2', '8', 'Ka Ching', '', '{E12FCE31-6280-4e9f-ACF4-F7425AB14D80}', 'shania_twain-ka_ching.mpg', 8, '1', 030722202520, NULL);
INSERT INTO project VALUES (45, 'Queen', '12', 'I am going slightly mad', '', '{2705B386-D2D8-4d47-A053-EB88DC79FA8B}', 'Queen - Im Going Slightly Mad.mpg', 8, '1', 030722202521, NULL);
INSERT INTO project VALUES (37, 'Bina animasyonu 1', '7', 'Textured + Wireframe', '', '{DDC78E1E-C82B-481e-B82A-2581F7132552}', 'Mix2.avi', 8, '1', 030722202521, NULL);
INSERT INTO project VALUES (38, 'Bina animasyonu 2', '7', 'Textured + Wireframe together', '', '{D1EBB055-DDFC-4893-9773-38DD8B1E8A42}', 'Mix3.avi', 8, '1', 030722202521, NULL);
INSERT INTO project VALUES (39, 'Shania Twain', '8', 'Ka-Ching', '', '{7DB7586A-2530-4c37-A103-EC1B69A897C3}', 'Shania Twain-Ka Ching! (European Video Release).mpg', 8, '1', 030722202521, NULL);
INSERT INTO project VALUES (44, 'Madonna', '11', 'You will see', '', '{465BF10D-8CEF-42a9-B4E7-398DFF521A2C}', 'Madonna - You will  See.mpg', 8, '1', 030722202521, NULL);
INSERT INTO project VALUES (41, 'rosey', '5', 'love', 'audio only', '{6D4B3F1A-4FCB-4407-BC54-208A155EDCBF}', 'Rosey - love.mp3', 8, '1', 030722202522, NULL);
INSERT INTO project VALUES (43, 'Enya', '10', 'Only Time', '', '{9054A035-E342-4e26-8FB3-FCD9ACC4B4EA}', 'Enya - Only Time.mpg', 8, '1', 030722202522, NULL);
INSERT INTO project VALUES (55, 'Nirvana', '15', 'Where did you sleep last night', '', '{B5262481-438D-4348-A1A9-B6E0B029EFC3}', 'Nirvana - sleep last night.mp3', 8, '1', 030722202523, NULL);
INSERT INTO project VALUES (56, 'Judas', '16', 'Pain Killer', '', '{2B471F9C-E16B-4fcd-B83E-7C6D731BE0AA}', 'Judas Priest - Painkiller.mp3', 8, '1', 030722202523, NULL);
INSERT INTO project VALUES (57, 'Alive', '17', 'Video', '', '{18A01E30-6165-4768-9CCD-FF97AA04C9C3}', 'POD-alive.mpeg', 8, '1', 030722202524, NULL);
INSERT INTO project VALUES (58, 'travis', '13', 'test', '', '{A7EB5A28-2B16-4924-BD3D-4AF22A1DC346}', 'Travis - Sing - XF.mpg', 8, '1', 030722202525, NULL);
INSERT INTO project VALUES (59, 'door', '6', 'people', '', '{27817A49-61EE-4810-BB4D-4970CFCFA67F}', 'Doors_-_People_Are_Strange.mpg', 8, '1', 030722202525, NULL);
INSERT INTO project VALUES (60, 'Guitar lesson', '18', 'Yngwie Malmsteen', '', '{120F1AFD-50F3-460a-BA3A-125FCFBB4288}', 'Guitar Lesson - Yngwie Malmsteen - Hot Licks.mpg', 8, '1', 030722202526, NULL);
INSERT INTO project VALUES (61, 'tori', '5', 'amos', '', '{CC65DE0D-F81F-4b24-8955-55DF85D765E4}', 'Tori Amos - Cornflake Girl.mpg', 8, '1', 030722202527, NULL);
INSERT INTO project VALUES (62, 'Bring me to life', '19', 'EVANESCENCE', '', '{519F34E6-E178-462d-ABA0-7CE15BC9AC2B}', 'Evanscence - Bring Me To Life.wmv', 8, '1', 030722202527, NULL);
INSERT INTO project VALUES (63, NULL, NULL, NULL, NULL, NULL, NULL, 8, '1', 030722202528, NULL);
# --------------------------------------------------------

#
# Tablo için tablo yapýsý `sessions`
#

CREATE TABLE sessions (
  id int(10) unsigned NOT NULL auto_increment,
  sesid varchar(50) NOT NULL default '',
  userid int(1) NOT NULL default '0',
  time timestamp(14) NOT NULL,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Tablo döküm verisi `sessions`
#

INSERT INTO sessions VALUES (6, 'd8001acdfaca9d59d1d0717542d8d6c7', 8, 20030711175235);
# --------------------------------------------------------

#
# Tablo için tablo yapýsý `statistics`
#

CREATE TABLE statistics (
  id int(1) unsigned NOT NULL auto_increment,
  pid int(1) unsigned NOT NULL default '0',
  numofrev int(11) unsigned NOT NULL default '0',
  numofunrev int(11) unsigned NOT NULL default '0',
  recorddate timestamp(12) NOT NULL,
  note text,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Tablo döküm verisi `statistics`
#

# --------------------------------------------------------

#
# Tablo için tablo yapýsý `stdvscrs`
#

CREATE TABLE stdvscrs (
  id int(1) unsigned NOT NULL auto_increment,
  courseid int(1) unsigned NOT NULL default '0',
  userid int(1) unsigned NOT NULL default '0',
  recorddate timestamp(12) NOT NULL,
  note text,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Tablo döküm verisi `stdvscrs`
#

# --------------------------------------------------------

#
# Tablo için tablo yapýsý `system`
#

CREATE TABLE system (
  id int(1) unsigned NOT NULL auto_increment,
  cookiename varchar(100) NOT NULL default '',
  exptime varchar(15) NOT NULL default '',
  hkey varchar(100) NOT NULL default '',
  myserver varchar(50) NOT NULL default '',
  myuser varchar(50) NOT NULL default '',
  mypass varchar(50) NOT NULL default '',
  mydatabase varchar(50) NOT NULL default '',
  cemail varchar(100) NOT NULL default '',
  eemail varchar(100) NOT NULL default '',
  faqemail varchar(100) NOT NULL default '',
  adminname varchar(100) NOT NULL default '',
  recorddate timestamp(14) NOT NULL,
  note text NOT NULL,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Tablo döküm verisi `system`
#

INSERT INTO system VALUES (1, 'VoD', '25000', 'caner', '', '', '', '', 'cem.karaca@emu.edu.tr', 'cem.karaca@emu.edu.tr', 'cem.karaca@emu.edu.tr', 'Cem KARACA', 20030715132523, '');
# --------------------------------------------------------

#
# Tablo için tablo yapýsý `user`
#

CREATE TABLE user (
  userid int(1) unsigned NOT NULL auto_increment,
  user varchar(50) default NULL,
  pass varchar(50) default NULL,
  privilege char(1) default NULL,
  title varchar(50) default NULL,
  name varchar(50) default NULL,
  surname varchar(50) default NULL,
  mail varchar(50) default NULL,
  status char(1) default '1',
  recorddate timestamp(12) NOT NULL,
  note text,
  PRIMARY KEY  (userid)
) TYPE=MyISAM;

#
# Tablo döküm verisi `user`
#

INSERT INTO user VALUES (8, 'emre', 'XXX', '2', 'Mr.', 'Emre', 'OKTEM', 'emre.oktem@emu.edu.tr', '1', 030724232032, NULL);
INSERT INTO user VALUES (14, 'cem', 'XXX', '2', 'Mr.', 'Cem', 'KARACA', 'ckaraca@ikikule.com', '1', 030724232800, NULL);
INSERT INTO user VALUES (15, 'control', '3fb3a05c165090cd', '2', 'Mr.', 'Kontrol', 'Test', 'test@test.com', '1', 031201224304, NULL);

