-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- 主机： localhost
-- 生成日期： 2024-06-01 16:11:24
-- 服务器版本： 5.7.44-log
-- PHP 版本： 7.4.33

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- 数据库： `cyberpodium`
--

-- --------------------------------------------------------

--
-- 表的结构 `attendance`
--

CREATE TABLE `attendance` (
  `用户ID` int(11) NOT NULL,
  `昵称` varchar(255) DEFAULT NULL,
  `课程ID` int(11) NOT NULL,
  `最近更新时间` date NOT NULL,
  `记录` varchar(255) NOT NULL DEFAULT '0',
  `教师备注` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- 转存表中的数据 `attendance`
--

INSERT INTO `attendance` (`用户ID`, `昵称`, `课程ID`, `最近更新时间`, `记录`, `教师备注`) VALUES
(1, '开发者', 240000001, '2024-05-05', '0,0,0,0,0,0', '全勤'),
(1, '开发者', 240000002, '2024-04-27', '0,0,0', NULL),
(1, '开发者', 240000003, '2024-04-20', '0', NULL),
(1, '开发者', 240000004, '2024-04-20', '0', NULL),
(2, '安卓', 240000001, '2024-05-05', '0,1,1,1,1,1', '全勤'),
(2, '安卓', 240000002, '2024-04-27', '0,0,0', NULL),
(2, '', 240000006, '2024-05-05', '0', '哈哈'),
(240300000, '林铭镕', 240000001, '2024-05-05', '0,0,0,0,0,0', '挂科'),
(240300001, '林铭镕', 240000002, '2024-04-27', '0,0,0', NULL),
(240400000, '林铭镕', 240000001, '2024-05-05', '0,0,0,0,0,0', '修改备注并保存'),
(240500000, '', 240000001, '2024-05-05', '1', '');

-- --------------------------------------------------------

--
-- 表的结构 `connection`
--

CREATE TABLE `connection` (
  `userID` int(11) NOT NULL,
  `courseID` int(11) NOT NULL,
  `fd` int(11) NOT NULL,
  `isTeacher` int(1) NOT NULL DEFAULT '0',
  `date` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- 表的结构 `coursetable`
--

CREATE TABLE `coursetable` (
  `课程ID` int(11) NOT NULL,
  `课程名` varchar(255) COLLATE utf8mb4_unicode_ci NOT NULL,
  `时间` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `地点` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `创建者` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `备注` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci ROW_FORMAT=DYNAMIC;

--
-- 转存表中的数据 `coursetable`
--

INSERT INTO `coursetable` (`课程ID`, `课程名`, `时间`, `地点`, `创建者`, `备注`) VALUES
(240000001, '高等数学', '周四上午', '北404', '开发者', '修改并保存了《高等数学》的备注！！'),
(240000002, '线性代数', '周二上午第一二节', '一课北504', '开发者', '20网工班'),
(240000003, '安卓开发', '周五下午', '学院', '安卓开发', ''),
(240000004, '计算机网络', '周五下午', '北204', '林铭镕', ''),
(240000005, '计算机组成原理', '周三晚上', '南404', '林铭镕', ''),
(240000006, 'Database', '周日上午', '北304', '开发者', '');

-- --------------------------------------------------------

--
-- 表的结构 `studentcoursetable`
--

CREATE TABLE `studentcoursetable` (
  `用户ID` int(11) NOT NULL COMMENT '学生用户ID',
  `课程ID` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci ROW_FORMAT=DYNAMIC;

--
-- 转存表中的数据 `studentcoursetable`
--

INSERT INTO `studentcoursetable` (`用户ID`, `课程ID`) VALUES
(1, 240000001),
(2, 240000001),
(240300000, 240000001),
(240400000, 240000001),
(240500000, 240000001),
(1, 240000002),
(2, 240000002),
(240300001, 240000002),
(1, 240000003),
(1, 240000004),
(2, 240000006);

-- --------------------------------------------------------

--
-- 表的结构 `teachercoursetable`
--

CREATE TABLE `teachercoursetable` (
  `用户ID` int(11) NOT NULL COMMENT '教师或助教用户ID',
  `课程ID` int(11) NOT NULL,
  `助教备注` tinyint(1) DEFAULT NULL COMMENT '1表示这是助教|其他值表示这是老师'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci ROW_FORMAT=DYNAMIC;

--
-- 转存表中的数据 `teachercoursetable`
--

INSERT INTO `teachercoursetable` (`用户ID`, `课程ID`, `助教备注`) VALUES
(1, 240000001, NULL),
(1, 240000002, 1),
(1, 240000006, 0),
(2, 240000003, 0),
(240300001, 240000004, 0),
(240300001, 240000005, 0);

-- --------------------------------------------------------

--
-- 表的结构 `usertable`
--

CREATE TABLE `usertable` (
  `用户ID` int(11) NOT NULL,
  `用户名` varchar(255) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'default',
  `密码` varchar(255) COLLATE utf8mb4_unicode_ci NOT NULL,
  `微信号` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `QQ` varchar(20) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `邮箱` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `电话` varchar(40) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `办公室` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `备注` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci ROW_FORMAT=DYNAMIC;

--
-- 转存表中的数据 `usertable`
--

INSERT INTO `usertable` (`用户ID`, `用户名`, `密码`, `微信号`, `QQ`, `邮箱`, `电话`, `办公室`, `备注`) VALUES
(1, '开发者', '1', '000', '111', '119', '111', '华南师范大学行政楼', '修改用户信息'),
(2, '安卓', '2', '31414514', '145414512', '314214344', '213541255', '145234513', '143513451'),
(240300000, '林铭镕', '123456', NULL, NULL, NULL, NULL, NULL, NULL),
(240300001, '林铭镕', '123456', '1122', '21233', '133', '1333', '133', ''),
(240300002, 'Alice', '123456', NULL, NULL, NULL, NULL, NULL, NULL),
(240300003, 'Bob', '123456', NULL, NULL, NULL, NULL, NULL, NULL),
(240300004, 'Cindy', '123456', NULL, NULL, NULL, NULL, NULL, NULL),
(240300005, 'David', '123456', NULL, NULL, NULL, NULL, NULL, NULL),
(240300006, '李华', '0', NULL, NULL, NULL, NULL, NULL, NULL),
(240300007, '建国', '123456', NULL, NULL, NULL, NULL, NULL, NULL),
(240300008, '援朝', '123456', NULL, NULL, NULL, NULL, NULL, NULL),
(240300009, '小红', '123456', NULL, NULL, NULL, NULL, NULL, NULL),
(240300010, '小明', '123456', NULL, NULL, NULL, NULL, NULL, NULL),
(240300011, '李白', '123456', NULL, NULL, NULL, NULL, NULL, NULL),
(240300012, '李清照', '123456', NULL, NULL, NULL, NULL, NULL, NULL),
(240300013, '王维', '123456', NULL, NULL, NULL, NULL, NULL, NULL),
(240300014, '杜甫', '123456', NULL, NULL, NULL, NULL, NULL, NULL),
(240300015, '孟浩然', '123456', NULL, NULL, NULL, NULL, NULL, NULL),
(240400000, '林铭镕', '123456', NULL, NULL, NULL, NULL, NULL, NULL),
(240500000, '林肯', '123455', NULL, NULL, NULL, NULL, NULL, NULL);

--
-- 转储表的索引
--

--
-- 表的索引 `attendance`
--
ALTER TABLE `attendance`
  ADD PRIMARY KEY (`用户ID`,`课程ID`),
  ADD KEY `课程ID` (`课程ID`);

--
-- 表的索引 `connection`
--
ALTER TABLE `connection`
  ADD PRIMARY KEY (`userID`,`courseID`);

--
-- 表的索引 `coursetable`
--
ALTER TABLE `coursetable`
  ADD PRIMARY KEY (`课程ID`) USING BTREE;

--
-- 表的索引 `studentcoursetable`
--
ALTER TABLE `studentcoursetable`
  ADD PRIMARY KEY (`用户ID`,`课程ID`) USING BTREE,
  ADD KEY `学生课程课程ID` (`课程ID`) USING BTREE;

--
-- 表的索引 `teachercoursetable`
--
ALTER TABLE `teachercoursetable`
  ADD PRIMARY KEY (`用户ID`,`课程ID`) USING BTREE,
  ADD KEY `教师课程课程ID` (`课程ID`) USING BTREE;

--
-- 表的索引 `usertable`
--
ALTER TABLE `usertable`
  ADD PRIMARY KEY (`用户ID`) USING BTREE;

--
-- 限制导出的表
--

--
-- 限制表 `attendance`
--
ALTER TABLE `attendance`
  ADD CONSTRAINT `attendance_ibfk_1` FOREIGN KEY (`用户ID`) REFERENCES `usertable` (`用户ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `attendance_ibfk_2` FOREIGN KEY (`课程ID`) REFERENCES `coursetable` (`课程ID`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- 限制表 `studentcoursetable`
--
ALTER TABLE `studentcoursetable`
  ADD CONSTRAINT `学生课程用户ID` FOREIGN KEY (`用户ID`) REFERENCES `usertable` (`用户ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `学生课程课程ID` FOREIGN KEY (`课程ID`) REFERENCES `coursetable` (`课程ID`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- 限制表 `teachercoursetable`
--
ALTER TABLE `teachercoursetable`
  ADD CONSTRAINT `教师课程用户ID` FOREIGN KEY (`用户ID`) REFERENCES `usertable` (`用户ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `教师课程课程ID` FOREIGN KEY (`课程ID`) REFERENCES `coursetable` (`课程ID`) ON DELETE CASCADE ON UPDATE CASCADE;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
