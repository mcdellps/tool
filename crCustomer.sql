USE [tpcc]
GO

/****** Object:  Table [dbo].[CUSTOMER]    Script Date: 05/18/2017 15:51:15 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_PADDING ON
GO

CREATE TABLE [dbo].[CUSTOMER](
	[c_id] [int] NOT NULL,
	[c_d_id] [tinyint] NOT NULL,
	[c_w_id] [int] NOT NULL,
	[c_discount] [smallmoney] NULL,
	[c_credit_lim] [money] NULL,
	[c_last] [char](16) NULL,
	[c_first] [char](16) NULL,
	[c_credit] [char](2) NULL,
	[c_balance] [money] NULL,
	[c_ytd_payment] [money] NULL,
	[c_payment_cnt] [smallint] NULL,
	[c_delivery_cnt] [smallint] NULL,
	[c_street_1] [char](20) NULL,
	[c_street_2] [char](20) NULL,
	[c_city] [char](20) NULL,
	[c_state] [char](2) NULL,
	[c_zip] [char](9) NULL,
	[c_phone] [char](16) NULL,
	[c_since] [datetime] NULL,
	[c_middle] [char](2) NULL,
	[c_data] [char](500) NULL
) ON [PRIMARY]

GO

SET ANSI_PADDING OFF
GO

ALTER TABLE [dbo].[CUSTOMER] SET (LOCK_ESCALATION = DISABLE)
GO

