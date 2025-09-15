UPDATE instructor set salary = salary*1.1 where ID in (select temp.ID from (SELECT ID from department as d, instructor as i where d.dept_name = i.dept_name and budget > 900000) as temp);
