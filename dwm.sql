-- MySQL dump 10.14  Distrib 5.5.52-MariaDB, for Linux (i686)
--
-- Host: localhost    Database: dwm
-- ------------------------------------------------------
-- Server version	5.5.52-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `current_goods_table`
--

DROP TABLE IF EXISTS `current_goods_table`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `current_goods_table` (
  `goodsType` int(2) DEFAULT NULL,
  `goodsName` char(64) DEFAULT NULL,
  `goodsID` int(50) DEFAULT NULL,
  `putInTime` datetime(1) DEFAULT NULL,
  `goodsProperty` int(2) DEFAULT NULL,
  `goodsSIC` char(64) DEFAULT NULL,
  `cost` int(8) DEFAULT NULL,
  `costoff` char(2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `current_goods_table`
--

LOCK TABLES `current_goods_table` WRITE;
/*!40000 ALTER TABLE `current_goods_table` DISABLE KEYS */;
INSERT INTO `current_goods_table` VALUES (1,'Iphone X',1,'2017-09-27 05:51:04.0',1,'E2-00-41-25-22-07-01-79-11-80-9D-06',12000,'96'),(1,'Iphone 8',1,'2017-09-27 05:51:04.0',3,'E2-00-41-25-22-07-01-93-11-80-9D-3E',5800,'95'),(1,'Huawei RY6',1,'2017-09-27 05:51:04.0',18,'E2-00-41-25-22-07-01-78-11-80-9D-02',1800,'88');
/*!40000 ALTER TABLE `current_goods_table` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dwm_users_table`
--

DROP TABLE IF EXISTS `dwm_users_table`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dwm_users_table` (
  `username` char(64) DEFAULT NULL,
  `password` char(64) DEFAULT NULL,
  `type` int(1) DEFAULT NULL,
  `status` int(1) DEFAULT NULL,
  `currentlogincount` int(2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dwm_users_table`
--

LOCK TABLES `dwm_users_table` WRITE;
/*!40000 ALTER TABLE `dwm_users_table` DISABLE KEYS */;
INSERT INTO `dwm_users_table` VALUES ('admin','000000',0,0,0),('dwm','dwm',0,0,0);
/*!40000 ALTER TABLE `dwm_users_table` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `rfid_device_table`
--

DROP TABLE IF EXISTS `rfid_device_table`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rfid_device_table` (
  `deviceid` int(12) DEFAULT NULL,
  `rfiddevice` char(64) DEFAULT NULL,
  `leddevice` char(64) DEFAULT NULL,
  `logintime` char(64) DEFAULT NULL,
  `rfiddevicestatus` int(2) DEFAULT NULL,
  `leddevicestatus` int(2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `rfid_device_table`
--

LOCK TABLES `rfid_device_table` WRITE;
/*!40000 ALTER TABLE `rfid_device_table` DISABLE KEYS */;
INSERT INTO `rfid_device_table` VALUES (55660,'3A:0D:0A:7E:7E:00','40:b8:9a:3f:e8:97','2017-09-27 05:51:04',0,0);
/*!40000 ALTER TABLE `rfid_device_table` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `selled_goods_table`
--

DROP TABLE IF EXISTS `selled_goods_table`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `selled_goods_table` (
  `goodsType` int(1) DEFAULT NULL,
  `goodsName` char(64) DEFAULT NULL,
  `goodsID` int(2) DEFAULT NULL,
  `putOutTime` datetime(1) DEFAULT NULL,
  `goodsProperty` int(2) DEFAULT NULL,
  `goodsSIC` char(32) DEFAULT NULL,
  `cost` int(2) DEFAULT NULL,
  `costoff` int(2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `selled_goods_table`
--

LOCK TABLES `selled_goods_table` WRITE;
/*!40000 ALTER TABLE `selled_goods_table` DISABLE KEYS */;
/*!40000 ALTER TABLE `selled_goods_table` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-10-18 13:08:43
