CCREATE view facload as (SELECT name,count(name) as course_no, sum(credits) as tot_credits FROM course as c, (select i.ID, t.course_id, i.name from instructor as i , teaches as t where i.ID = t.ID and year in (2017,2018)) as temp where c.course_id = temp.course_id group by name );
select * from facload;
