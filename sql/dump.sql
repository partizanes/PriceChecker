/*
SQLyog Ultimate v8.62 
MySQL - 5.1.62-community : Database - action
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`action` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `action`;

/*Table structure for table `action_price` */

DROP TABLE IF EXISTS `action_price`;

CREATE TABLE `action_price` (
  `barcode` bigint(13) NOT NULL,
  `price_old` bigint(6) NOT NULL,
  `price_new` bigint(6) NOT NULL,
  `start_action` date NOT NULL,
  `stop_action` date NOT NULL,
  `active` int(1) NOT NULL DEFAULT '1'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `action_price` */

/*Table structure for table `version` */

DROP TABLE IF EXISTS `version`;

CREATE TABLE `version` (
  `name` char(20) DEFAULT NULL,
  `parameter` int(3) DEFAULT NULL,
  `source` char(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `version` */

insert  into `version`(`name`,`parameter`,`source`) values ('PriceCheck',1,NULL),('PriceLogReceiver',1,NULL),('Action',1,NULL),('Discount',1,NULL);

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
