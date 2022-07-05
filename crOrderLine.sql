USE [tpcc]
GO

/****** Object:  Table [dbo].[ORDER_LINE]    Script Date: 05/18/2017 15:52:44 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_PADDING ON
GO

CREATE TABLE [dbo].[ORDER_LINE](
	[ol_o_id] [int] NOT NULL,
	[ol_d_id] [tinyint] NOT NULL,
	[ol_w_id] [int] NOT NULL,
	[ol_number] [tinyint] NOT NULL,
	[ol_i_id] [int] NULL,
	[ol_delivery_d] [datetime] NULL,
	[ol_amount] [smallmoney] NULL,
	[ol_supply_w_id] [int] NULL,
	[ol_quantity] [smallint] NULL,
	[ol_dist_info] [char](24) NULL
) ON [PRIMARY]

GO

SET ANSI_PADDING OFF
GO

ALTER TABLE [dbo].[ORDER_LINE] SET (LOCK_ESCALATION = DISABLE)
GO

