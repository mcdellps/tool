USE [SQLPROD-01]
GO

BULK INSERT customer FROM 'C:\Users\Administrator.DEMO\Desktop\customer_1.tbl' WITH (TABLOCK, DATAFILETYPE='char', CODEPAGE='raw', FIELDTERMINATOR = '|')
GO

-- INSERT INTO customer SELECT * FROM customer
-- GO
