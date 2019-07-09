<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8" import="java.sql.*"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
 <html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,shrink-to-fit=no">
 <title>2018 JBNU SKIGAME RANKING</title>
                
                <!-- Bootstrap CSS -->
                <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-alpha.6/css/bootstrap.min.css" integrity="sha384-rwoIResjU2yc3z8GV/NPeZWAv56rSmLldC3R/AZzGRnGxQQKnKkoFVhFQhNUwEyJ" crossorigin="anonymous">
                <style> 
                        h1 {    
                                color:white;
                                font-weight:bold;
                                font-family:Fantasy;
                        }
                </style>
        </head>
<body style='background-color:black;'>
 <center>
                        <br><br><br>
                        <h1 style='font-size:60px;'>2018 JBNU SKI-GAME RANKING</h1>
                        <br><br><br>
                        <table class="table table-inverse">
                                <thead>
                                <tr>
                                        <th scope="col">Ranking No</th>
                                        <th scope="col">Name</th>
                                        <th scope="col">Score</th>
                                </tr>
                                </thead>
                                <tbody>

<%
String DB_URL = "****";
String DB_USER = "****";
String DB_PASSWORD= "****";
String query="SELECT sno,name,score, (@rank := @rank+1) AS rank, (@real_rank := IF(@last>score,@real_rank:=@real_rank+1,@real_rank)) AS real_rank, (@last := score) FROM score_tb AS a, (SELECT @rank:=0,@last:=0,@real_rank:=1) AS b ORDER BY a.score DESC;";
Connection conn=null;
Statement stmt=null;
ResultSet rs =null;

try {
 Class.forName("com.mysql.jdbc.Driver");
 conn = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD);
 stmt = conn.createStatement();
 rs = stmt.executeQuery(query);
 while(rs.next()){
%>
 <tr>
  <th scope="row"><%=rs.getString("real_rank") %> </th>
  <td> <%=rs.getString("name") %></td>
  <td> <%=rs.getString("score") %></td>
</tr>
<%
} 
}catch(SQLException ex) {
 out.println(ex.getMessage());
 ex.printStackTrace();
}finally{
	if(rs!=null) try{rs.close();} catch(SQLException ex) {}
	if(stmt!=null) try{stmt.close();} catch(SQLException ex) {}
	if(conn!=null) try{conn.close();} catch(SQLException ex) {}
}
%>
		</tbody>
	</table>
<!-- Footer -->
                        <footer class="page-footer font-small grey">

                        <!-- Copyright -->
                        <div class="footer-copyright text-center py-3">�� 2018 Copyright: JBNU Unix Programming Project ShortTurn!
                        </div>
                        </footer>
                </center>

</body>
</html>
