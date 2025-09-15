SELECT f.name, course_no, tot_credits, dept_name from facload as f, instructor as i where f.name = i.name order by f.tot_credits desc LIMIT 1;
