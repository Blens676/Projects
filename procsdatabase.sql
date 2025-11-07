go


 ------------------------------------------EMPLOYEE--------------------------------
--1 insert 


ALTER TABLE employees
ADD CONSTRAINT UQ_email UNIQUE (email);
go
CREATE PROCEDURE InsertEmployee
    @first_name NVARCHAR(50),
    @last_name NVARCHAR(50),
    @email VARCHAR(100),
    @phone_number VARCHAR(20),
    @hire_date DATE,
    @gender CHAR = 'M',  -- Default value is 'M'
    @is_active BIT = 1   -- Default value is 1
AS
BEGIN
    -- Begin transaction
    BEGIN TRANSACTION

    BEGIN TRY
        -- Check if phone number is exactly 9 digits
        IF LEN(@phone_number) <> 9 OR ISNUMERIC(@phone_number) = 0
        BEGIN
            RAISERROR('Phone number must be exactly 9 digits', 16, 1)
            ROLLBACK TRANSACTION
            RETURN
        END

        -- Check if email already exists
        IF EXISTS (SELECT 1 FROM employees WHERE email = @email)
        BEGIN
            RAISERROR('Email already exists', 16, 1)
            ROLLBACK TRANSACTION
            RETURN
        END

        -- Check if phone number already exists
        IF EXISTS (SELECT 1 FROM employees WHERE phone_number = @phone_number)
        BEGIN
            RAISERROR('Phone number already exists', 16, 1)
            ROLLBACK TRANSACTION
            RETURN
        END

        -- Insert new employee record
        INSERT INTO employees (first_name, last_name, email, phone_number, hire_date, gender, is_active, created_at, updated_at)
        VALUES (@first_name, @last_name, @email, @phone_number, @hire_date, @gender, @is_active, GETDATE(), GETDATE())

        -- Commit transaction
        COMMIT TRANSACTION
    END TRY
    BEGIN CATCH
        -- Rollback transaction if an error occurs
        ROLLBACK TRANSACTION

        -- Raise the error
        DECLARE @ErrorMessage NVARCHAR(4000), @ErrorSeverity INT, @ErrorState INT
        SELECT @ErrorMessage = ERROR_MESSAGE(), @ErrorSeverity = ERROR_SEVERITY(), @ErrorState = ERROR_STATE()
        RAISERROR (@ErrorMessage, @ErrorSeverity, @ErrorState)
    END CATCH
END
--test
go
DECLARE @first_name NVARCHAR(50) = 'lala';
DECLARE @last_name NVARCHAR(50) = 'trial';
DECLARE @hire_date DATE = '2024-06-01';
DECLARE @gender CHAR = 'f';
DECLARE @is_active BIT = 1;
DECLARE @email1 VARCHAR(100) = 'lalatrial.com';
DECLARE @phone_number1 VARCHAR(20) = '123456789';

EXEC InsertEmployee
    @first_name = @first_name,
    @last_name = @last_name,
    @email = @email1,
    @phone_number = @phone_number1,
    @hire_date = @hire_date,
    @gender = @gender,
    @is_active = @is_active;
	
	
	go
	ALTER TABLE customers
ADD CONSTRAINT UQc_custphone_number UNIQUE (customers_phone);
go

ALTER TABLE customers
ADD CONSTRAINT UQ_custemail UNIQUE (customers_email);
go



--- EMPLOYEEE UPDATE
CREATE PROCEDURE UpdateEmployeeById
    @employee_id INT,
    @new_first_name NVARCHAR(50),
    @new_last_name NVARCHAR(50),
    @new_email VARCHAR(100),
    @new_phone_number VARCHAR(20),
    @new_hire_date DATE,
    @new_gender CHAR
AS
BEGIN
    SET NOCOUNT ON;

    -- Check if the new email or phone number already exists for another employee
    IF EXISTS (
        SELECT 1
        FROM employees
        WHERE (email = @new_email OR phone_number = @new_phone_number)
          AND employee_id <> @employee_id
    )
    BEGIN
        RAISERROR('Email or phone number already exists for another employee.', 16, 1);
        RETURN;
    END;

    -- Update the employee information
    UPDATE employees
    SET first_name = @new_first_name,
        last_name = @new_last_name,
        email = @new_email,
        phone_number = @new_phone_number,
        hire_date = @new_hire_date,
        gender = @new_gender,
        updated_at = GETDATE()
    WHERE employee_id = @employee_id;
END;
go
--delete employee
CREATE PROCEDURE DeactivateEmployeeById
    @employee_id INT
AS
BEGIN
    SET NOCOUNT ON;

    -- Check if the employee exists
    IF NOT EXISTS (
        SELECT 1
        FROM employees
        WHERE employee_id = @employee_id
    )
    BEGIN
        RAISERROR('Employee not found.', 16, 1);
        RETURN;
    END;

    -- Update the is_active column to 0
    UPDATE employees
    SET is_active = 0
    WHERE employee_id = @employee_id;
END;

go


CREATE VIEW EmployeeView 
AS
SELECT
    employee_id,
    first_name,
    last_name,
    email,
	  is_active
FROM
    employees;






















-----------------------------CUSTOMERS------------------------------------

go

CREATE PROCEDURE InsertCustomer
    @customers_first_name NVARCHAR(50),
    @customers_last_name NVARCHAR(50),
    @customers_email VARCHAR(100),
    @customers_phone VARCHAR(20),
    @customers_address NVARCHAR(200),
    @customers_gen CHAR = 'M',  -- Default value is 'M'
    @customers_is_active BIT = 1   -- Default value is 1
AS
BEGIN
    -- Begin transaction
    BEGIN TRANSACTION

    BEGIN TRY
        -- Check if phone number is exactly 9 digits and numeric
        IF LEN(@customers_phone) <> 9 OR ISNUMERIC(@customers_phone) = 0
        BEGIN
            RAISERROR('Phone number must be exactly 9 digits and numeric', 16, 1)
            ROLLBACK TRANSACTION
            RETURN
        END

        -- Check if email already exists
        IF EXISTS (SELECT 1 FROM customers WHERE customers_email = @customers_email)
        BEGIN
            RAISERROR('Email already exists', 16, 1)
            ROLLBACK TRANSACTION
            RETURN
        END

        -- Check if phone number already exists
        IF EXISTS (SELECT 1 FROM customers WHERE customers_phone = @customers_phone)
        BEGIN
            RAISERROR('Phone number already exists', 16, 1)
            ROLLBACK TRANSACTION
            RETURN
        END

        -- Insert new customer record
        INSERT INTO customers (customers_first_name, customers_last_name, customers_email, customers_phone, customers_address, customers_gen, customers_is_active, customers_created_at, customers_updated_at)
        VALUES (@customers_first_name, @customers_last_name, @customers_email, @customers_phone, @customers_address, @customers_gen, @customers_is_active, GETDATE(), GETDATE())

        -- Commit transaction
        COMMIT TRANSACTION
    END TRY
    BEGIN CATCH
        -- Rollback transaction if an error occurs
        ROLLBACK TRANSACTION

        -- Raise the error
        DECLARE @ErrorMessage NVARCHAR(4000), @ErrorSeverity INT, @ErrorState INT
        SELECT @ErrorMessage = ERROR_MESSAGE(), @ErrorSeverity = ERROR_SEVERITY(), @ErrorState = ERROR_STATE()
        RAISERROR (@ErrorMessage, @ErrorSeverity, @ErrorState)
    END CATCH
END
--trial
go
DECLARE @customers_first_name NVARCHAR(50) = 'Jane';
DECLARE @customers_last_name NVARCHAR(50) = 'Doe';
DECLARE @customers_email VARCHAR(100) = 'jane.doe@example.com';
DECLARE @customers_phone VARCHAR(20) = '123456789';  -- 9-digit phone number
DECLARE @customers_address NVARCHAR(200) = '123 Main St';
DECLARE @customers_gen CHAR = 'F';
DECLARE @customers_is_active BIT = 1;

EXEC InsertCustomer
    @customers_first_name = @customers_first_name,
    @customers_last_name = @customers_last_name,
    @customers_email = @customers_email,
    @customers_phone = @customers_phone,
    @customers_address = @customers_address,
    @customers_gen = @customers_gen,
    @customers_is_active = @customers_is_active;
	go


	CREATE PROCEDURE SearchCustomerByFirstName
    @FirstName NVARCHAR(50)
AS
BEGIN
    SET NOCOUNT ON;

    SELECT
        customer_id,
        customers_first_name,
        customers_last_name,
         customers_phone,
         customers_address,
        customers_is_active
      
    FROM
        customers
    WHERE
        customers_first_name LIKE '%' + @FirstName + '%';
END
go
  ----update customer 
  CREATE PROCEDURE UpdateCustomerBasic
    @CustomerID INT,
    @FirstName NVARCHAR(50),
    @LastName NVARCHAR(50),
    @Email NVARCHAR(100),
    @Gender NVARCHAR(10)

AS
BEGIN
    SET NOCOUNT ON;

    -- Check if the customer ID exists
    IF NOT EXISTS (SELECT 1 FROM customers WHERE customer_id = @CustomerID)
    BEGIN
        RAISERROR('Customer ID does not exist.', 16, 1);
        RETURN;
    END

    -- Check if the new email is unique
    IF EXISTS (SELECT 1 FROM customers WHERE  customers_email = @Email AND customer_id <> @CustomerID)
    BEGIN
        RAISERROR('The email address is already in use.', 16, 1);
        RETURN;
    END

   

    -- Update the customer information
    UPDATE customers
    SET customers_first_name = @FirstName,
        customers_last_name = @LastName,
        
        customers_gen = @Gender,
       
        customers_updated_at = GETDATE()
    WHERE customer_id = @CustomerID;
END

go

CREATE PROCEDURE UpdateCustomerPhoneAndAddress
    @CustomerID INT,
    @Phone NVARCHAR(20),
    @Address NVARCHAR(200)
AS
BEGIN
    SET NOCOUNT ON;

    -- Check if the customer ID exists
    IF NOT EXISTS (SELECT 1 FROM customers WHERE customer_id = @CustomerID)
    BEGIN
        RAISERROR('Customer ID does not exist.', 16, 1);
        RETURN;
    END

    -- Check if the new phone number is unique
    IF EXISTS (SELECT 1 FROM customers WHERE customers_phone = @Phone AND customer_id <> @CustomerID)
    BEGIN
        RAISERROR('The phone number is already in use.', 16, 1);
        RETURN;
    END

    -- Update the customer's phone number and address
    UPDATE customers
    SET customers_phone = @Phone,
        customers_address = @Address,
        customers_updated_at = GETDATE()
    WHERE customer_id = @CustomerID;
END



go






CREATE PROCEDURE DeactivateCustomerByPhone
    @Phone NVARCHAR(20)
AS
BEGIN
    SET NOCOUNT ON;

    -- Check if the customer with the given phone number exists
    IF NOT EXISTS (SELECT 1 FROM customers WHERE customers_phone = @Phone)
    BEGIN
        RAISERROR('Customer with the given phone number does not exist.', 16, 1);
        RETURN;
    END

    -- Deactivate the customer
    UPDATE customers
    SET customers_is_active = 0,
        customers_updated_at = GETDATE()
    WHERE customers_phone = @Phone;
END
go
CREATE VIEW CustomerView AS
SELECT 
     customers_first_name, 
     customers_last_name,
     customers_email,
     customers_is_active
FROM 
    customers;














--vehicles
-- inserts 
go
CREATE PROCEDURE InsertVehicle
    @customer_id INT,
    @vehicles_make NVARCHAR(50),
    @vehicles_model NVARCHAR(50),
    @vehicles_year INT,
    @license_plate VARCHAR(20)
AS
BEGIN
    -- Begin transaction
    BEGIN TRANSACTION

    BEGIN TRY
        -- Check if customer_id exists
        IF NOT EXISTS (SELECT 1 FROM customers WHERE customer_id = @customer_id)
        BEGIN
            RAISERROR('Customer ID does not exist', 16, 1)
            ROLLBACK TRANSACTION
            RETURN
        END

        -- Check if license plate already exists
        IF EXISTS (SELECT 1 FROM vehicles WHERE license_plate = @license_plate)
        BEGIN
            RAISERROR('License plate already exists', 16, 1)
            ROLLBACK TRANSACTION
            RETURN
        END

        -- Insert new vehicle record
        INSERT INTO vehicles (customer_id, vehicles_make, vehicles_model, vehicles_year, license_plate, created_at, updated_at)
        VALUES (@customer_id, @vehicles_make, @vehicles_model, @vehicles_year, @license_plate, GETDATE(), GETDATE())

        -- Commit transaction
        COMMIT TRANSACTION
    END TRY
    BEGIN CATCH
        -- Rollback transaction if an error occurs
        ROLLBACK TRANSACTION

        -- Raise the error
        DECLARE @ErrorMessage NVARCHAR(4000), @ErrorSeverity INT, @ErrorState INT
        SELECT @ErrorMessage = ERROR_MESSAGE(), @ErrorSeverity = ERROR_SEVERITY(), @ErrorState = ERROR_STATE()
        RAISERROR (@ErrorMessage, @ErrorSeverity, @ErrorState)
    END CATCH
END

go
DECLARE @customer_id INT = 1; -- Assume this customer ID exists in the customers table
DECLARE @vehicles_make NVARCHAR(50) = 'Toyota';
DECLARE @vehicles_model NVARCHAR(50) = 'Camry';
DECLARE @vehicles_year INT = 2020;
DECLARE @license_plate VARCHAR(20) = 'ABC1234';  -- Unique license plate

EXEC InsertVehicle
    @customer_id = @customer_id,
    @vehicles_make = @vehicles_make,
    @vehicles_model = @vehicles_model,
    @vehicles_year = @vehicles_year,
    @license_plate = @license_plate;

	---update vehicles 
	go
	CREATE PROCEDURE UpdateVehicleByCustomerPhone
    @CustomerPhone NVARCHAR(20),
    @VehicleMake NVARCHAR(50),
    @VehicleModel NVARCHAR(50),
    @VehicleYear INT,
    @LicensePlate VARCHAR(20)
AS
BEGIN
    SET NOCOUNT ON;

    -- Check if the customer with the given phone number exists
    DECLARE @CustomerId INT;
    SELECT @CustomerId = customer_id 
    FROM customers
    WHERE customers_phone = @CustomerPhone;

    IF @CustomerId IS NULL
    BEGIN
        RAISERROR('Customer with the given phone number does not exist.', 16, 1);
        RETURN;
    END

    -- Check if the vehicle with the given license plate already exists
    IF EXISTS (SELECT 1 FROM vehicles WHERE license_plate = @LicensePlate)
    BEGIN
        RAISERROR('Vehicle with the given license plate already exists.', 16, 1);
        RETURN;
    END

    -- Update the vehicle information
    UPDATE vehicles
    SET customer_id = @CustomerId,
        vehicles_make = @VehicleMake,
        vehicles_model = @VehicleModel,
        vehicles_year = @VehicleYear,
        license_plate = @LicensePlate,
        updated_at = GETDATE()
    WHERE customer_id = @CustomerId;

    -- If no rows were updated, insert a new vehicle
    IF @@ROWCOUNT = 0
    BEGIN
        INSERT INTO vehicles (customer_id, vehicles_make, vehicles_model, vehicles_year, license_plate)
        VALUES (@CustomerId, @VehicleMake, @VehicleModel, @VehicleYear, @LicensePlate);
    END
END


go
ALTER TABLE vehicles
ADD vehicles_isactive BIT NOT NULL DEFAULT 1;


go
CREATE PROCEDURE deactivevehiclebyphonenumber
    @CustomerPhone NVARCHAR(20),
    @IsActive BIT
AS
BEGIN
    SET NOCOUNT ON;

    -- Check if the customer with the given phone number exists
    DECLARE @CustomerId INT;
    SELECT @CustomerId = customer_id 
    FROM customers
    WHERE customers_phone = @CustomerPhone;

    IF @CustomerId IS NULL
    BEGIN
        RAISERROR('Customer with the given phone number does not exist.', 16, 1);
        RETURN;
    END

    -- Update the vehicle active status
    UPDATE vehicles
    SET vehicles_isactive = 0,
        updated_at = GETDATE()
    WHERE customer_id = @CustomerId;
END
go


CREATE VIEW customer_vehicle_view AS
SELECT
    c.customers_first_name,
    c.customers_last_name,
    v.license_plate,
    v.vehicles_make,
    v.vehicles_model
FROM
    customers c
    JOIN vehicles v ON c.customer_id = v.customer_id
WHERE
    v.vehicles_isactive = 1;
	go


 




























-- parts
--inserts parts

go
CREATE PROCEDURE InsertPart
    @part_name NVARCHAR(100),
    @part_number VARCHAR(50),
    @part_description NVARCHAR(500),
    @part_price DECIMAL(10,2),
    @part_quantity INT
AS
BEGIN
    -- Begin transaction
    BEGIN TRANSACTION

    BEGIN TRY
        -- Check if part number already exists
        IF EXISTS (SELECT 1 FROM parts WHERE part_number = @part_number)
        BEGIN
            RAISERROR('Part number already exists', 16, 1)
            ROLLBACK TRANSACTION
            RETURN
        END

        -- Insert new part record
        INSERT INTO parts (part_name, part_number, part_description, part_price, part_quantity, created_at, updated_at)
        VALUES (@part_name, @part_number, @part_description, @part_price, @part_quantity, GETDATE(), GETDATE())

        -- Commit transaction
        COMMIT TRANSACTION
    END TRY
    BEGIN CATCH
        -- Rollback transaction if an error occurs
        ROLLBACK TRANSACTION

        -- Raise the error
        DECLARE @ErrorMessage NVARCHAR(4000), @ErrorSeverity INT, @ErrorState INT
        SELECT @ErrorMessage = ERROR_MESSAGE(), @ErrorSeverity = ERROR_SEVERITY(), @ErrorState = ERROR_STATE()
        RAISERROR (@ErrorMessage, @ErrorSeverity, @ErrorState)
    END CATCH
END


go 
DECLARE @part_name NVARCHAR(100) = 'Brake Pad';
DECLARE @part_number VARCHAR(50) = 'BP12345';  -- Unique part number
DECLARE @part_description NVARCHAR(500) = 'High-quality brake pad for cars';
DECLARE @part_price DECIMAL(10,2) = 29.99;
DECLARE @part_quantity INT = 100;

EXEC InsertPart
    @part_name = @part_name,
    @part_number = @part_number,
    @part_description = @part_description,
    @part_price = @part_price,
    @part_quantity = @part_quantity;


	go

	--update part
CREATE PROC increase_quantity
    @added_quantity INT,
    @partnum INT
AS
BEGIN
    -- Check if the part number exists
    IF EXISTS (SELECT 1 FROM parts WHERE part_number = @partnum)
    BEGIN
        -- Update the part quantity
        UPDATE parts
        SET part_quantity += @added_quantity,
            updated_at = GETDATE()
        WHERE part_number = @partnum;
    END
    ELSE
    BEGIN
        RAISERROR('Part number does not exist.', 16, 1);
    END
END
go


	--update part
CREATE PROC change_price
    @newprice decimal,
    @partnum INT
AS
BEGIN
    -- Check if the part number exists
    IF EXISTS (SELECT 1 FROM parts WHERE part_number = @partnum)
    BEGIN
        -- Update the part quantity
        UPDATE parts
        SET part_price=@newprice,
            updated_at = GETDATE()
        WHERE part_number = @partnum;
    END
    ELSE
    BEGIN
        RAISERROR('Part number does not exist.', 16, 1);
    END
END
go

CREATE VIEW view_parts
AS
SELECT
    part_name,
    part_number,
    part_price,
    part_quantity
FROM
    parts;
	go
	ALTER TABLE parts
ADD parts_isactive BIT NOT NULL DEFAULT 1;



go
CREATE VIEW view_low_quantity_parts
AS
SELECT
    part_name,
    part_number,
    part_price,
    part_quantity
FROM
    parts
	where  part_quantity < 10
	go

CREATE PROCEDURE deactive_parts
    @part_number INT
AS
BEGIN
    SET NOCOUNT ON;

    -- Check if the part number exists
    IF NOT EXISTS (SELECT 1 FROM parts WHERE part_number = @part_number)
    BEGIN
        RAISERROR('Part number does not exist.', 16, 1);
        RETURN;
    END;

    -- Deactivate the part by setting the is_active flag to 0
    UPDATE parts
    SET parts_isactive = 0
    WHERE part_number = @part_number;
END


















	-----------------------------SERVICE --------------------------------------

	CREATE PROCEDURE InsertGService
    @vehicle_id INT,
    @employee_id INT,
    @service_type NVARCHAR(100),
    @service_date DATE,
    @service_description NVARCHAR(500),
    @total_cost DECIMAL(10,2)
AS
BEGIN
    -- Begin transaction
    BEGIN TRANSACTION

    BEGIN TRY
        -- Check if vehicle_id exists
        IF NOT EXISTS (SELECT 1 FROM vehicles WHERE vehicle_id = @vehicle_id)
        BEGIN
            RAISERROR('Vehicle ID does not exist', 16, 1)
            ROLLBACK TRANSACTION
            RETURN
        END

        -- Check if employee_id exists
        IF NOT EXISTS (SELECT 1 FROM employees WHERE employee_id = @employee_id)
        BEGIN
            RAISERROR('Employee ID does not exist', 16, 1)
            ROLLBACK TRANSACTION
            RETURN
        END

        -- Insert new service record
        INSERT INTO gservice (vehicle_id, employee_id, service_type, service_date, service_description, total_cost, created_at, updated_at)
        VALUES (@vehicle_id, @employee_id, @service_type, @service_date, @service_description, @total_cost, GETDATE(), GETDATE())

        -- Commit transaction
        COMMIT TRANSACTION
    END TRY
    BEGIN CATCH
        -- Rollback transaction if an error occurs
        ROLLBACK TRANSACTION

        -- Raise the error
        DECLARE @ErrorMessage NVARCHAR(4000), @ErrorSeverity INT, @ErrorState INT
        SELECT @ErrorMessage = ERROR_MESSAGE(), @ErrorSeverity = ERROR_SEVERITY(), @ErrorState = ERROR_STATE()
        RAISERROR (@ErrorMessage, @ErrorSeverity, @ErrorState)
    END CATCH
END
go 
DECLARE @vehicle_id INT = 1; -- Assume this vehicle ID exists in the vehicles table
DECLARE @employee_id INT = 1; -- Assume this employee ID exists in the employees table
DECLARE @service_type NVARCHAR(100) = 'Oil Change';
DECLARE @service_date DATE = '2024-06-15';
DECLARE @service_description NVARCHAR(500) = 'Performed oil change and filter replacement.';
DECLARE @total_cost DECIMAL(10,2) = 50.00;

EXEC InsertGService
    @vehicle_id = @vehicle_id,
    @employee_id = @employee_id,
    @service_type = @service_type,
    @service_date = @service_date,
    @service_description = @service_description,
    @total_cost = @total_cost;

	go


	CREATE TRIGGER InsertGServicePartTrigger
ON gservice
AFTER INSERT
AS
BEGIN
    -- Insert records into gservice_part for each inserted gservice record
    INSERT INTO gservice_part (gservice_id, part_id, quantity, created_at, updated_at)
    SELECT 
        i.service_id, -- Assuming the primary key of gservice table is service_id
        p.part_id,
        gp.quantity, -- Quantity from the gservice_part table
        GETDATE(),
        GETDATE()
    FROM
        inserted i
    INNER JOIN
        parts p ON 1 = 1 -- Cartesian product with an INNER JOIN
    INNER JOIN
        gservice_part gp ON gp.gservice_id = i.service_id;
END;








---views
go
CREATE VIEW loyal_customers AS
SELECT
    c.customer_id,
    c.customers_first_name + ' ' + c.customers_last_name AS customer_name,
    c.customers_phone,
    c.customers_address,
    COUNT(gs.service_id) AS service_count
FROM
    customers c
    JOIN vehicles v ON c.customer_id = v.customer_id
    JOIN gservice gs ON v.vehicle_id = gs.vehicle_id
GROUP BY
    c.customer_id, c.customers_first_name, c.customers_last_name, c.customers_phone, c.customers_address
HAVING
    COUNT(gs.service_id) > 3
go
CREATE PROCEDURE GetLoyalCustomers
AS
BEGIN
    SELECT *
    FROM loyal_customers
    ORDER BY service_count DESC;
END

go
 
go
-- ser viwe
CREATE VIEW ServiceDetailView AS
SELECT
    g.service_id,
    c.customer_id,
    c.customers_first_name AS customer_fname,
    c.customers_last_name AS customer_lname,
    e.employee_id,
    e. first_name AS employee_fname,
    e. last_name AS employee_lname,
    v.license_plate,
    gp.part_id,
    p.part_name,
    g.unit_cost,
    p.part_price,
    gp.quantity,
    (g.unit_cost + SUM(p.part_price * gp.quantity)) AS total_cost,
    g.service_type,
    g.service_date,
    g.service_description
FROM
    gservice g
JOIN
    gservice_part gp ON g.service_id = gp.gservice_id
JOIN
    parts p ON gp.part_id = p.part_id
JOIN
    vehicles v ON g.vehicle_id = v.vehicle_id
JOIN
    employees e ON g.employee_id = e.employee_id
JOIN
    customers c ON v.customer_id = c.customer_id
GROUP BY
    g.service_id,
    c.customer_id,
    c.customers_first_name,
    c.customers_last_name,
    e.employee_id,
    e.  first_name,
    e. last_name,
    v.license_plate,
    gp.part_id,
    p.part_name,
    g.unit_cost,
    p.part_price,
    gp.quantity,
    g.service_type,
    g.service_date,
    g.service_description;




















go
CREATE INDEX idx_employee_phone ON employees (phone_number);
GO

CREATE INDEX idx_part_part_number ON parts (part_number);
GO 

CREATE INDEX idx_customer_phone ON customers (customers_phone);
GO

CREATE INDEX idx_vehicle_license_plate ON vehicles (license_plate);
GO

go
CREATE TABLE users (
    user_id INT IDENTITY(1,1) PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARbinary(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    phone_number VARCHAR(20) NOT NULL,
    role CHAR(1) NOT NULL DEFAULT 'U', -- 'A' for admin, 'U' for regular user
    created_at DATETIME2 DEFAULT GETDATE(),
    updated_at DATETIME2 DEFAULT GETDATE()
);
go

INSERT INTO users (username, password, email, phone_number, role)
VALUES 
    ('lala', CONVERT(VARBINARY(100), '123'), 'john@examplwer.com', '123456781', 'U'),
    ('lala2', CONVERT(VARBINARY(100), '1234'), 'jane@examplasdf.com', '098765437', 'a'),
    ('admin_user', CONVERT(VARBINARY(100), 'adminpass'), 'admin@example.com', '5551234567', 'A'),
    ('test_user1', CONVERT(VARBINARY(100), 'testpass1'), 'test1@example.com', '1112223333', 'U'),
    ('test_user2', CONVERT(VARBINARY(100), 'testpass2'), 'test2@example.com', '4445556666', 'U');



	go
	
go
CREATE PROCEDURE loginins
    @username VARCHAR(50),
    @password VARCHAR(100),
    @role CHAR(1) OUTPUT
AS
BEGIN
    SET NOCOUNT ON;

    DECLARE @hashed_password VARCHAR(100) = dbo.HashPassword(@password);

    -- Check if the user exists and the password is correct
    SELECT @role = role
    FROM users
    WHERE username = @username
    AND password = @hashed_password;

    -- If the user doesn't exist or the password is incorrect, set the role to 'N'
    IF @role IS NULL
    BEGIN
        SET @role = 'e';
    END
END







































 
 