import java.sql.*;
import java.util.Scanner;

public class JavaDatabase {
   // JDBC driver name and database URL
   static final String JDBC_DRIVER = "com.mysql.jdbc.Driver";  
   static final String DB_URL = "jdbc:mysql://localhost/ebookshop";

   //  Database credentials
   static final String USER = "root";
   static final String PASS = "";
   
   public static void main(String[] args) {
   Connection conn = null;   //database connect variable
   Statement stmt = null;    //database statement variable
   int menuinput = 0;    //flag for recursive menu
   try{
	  Class.forName("com.mysql.jdbc.Driver");

      int menuexit = 0;    //set menu flag to 0
      System.out.println("Connecting to database...");
      conn = DriverManager.getConnection(DB_URL,USER,PASS);

      while(menuexit == 0)
      {
    	  //menu for database manipulation
        System.out.println("Select what you would like to do with the database:");
        System.out.println("(Select a number 1 - 5)");
        System.out.println("1: Show database");
        System.out.println("2: Insert to database");
        System.out.println("3: Delete from database (by book name)");
        System.out.println("4: Search database (by book name)");
        System.out.println("5: Exit");
        Scanner scanner = new Scanner(System.in);
      
        //only accept integers for menu options
        if(scanner.hasNextInt()){
        	menuinput = scanner.nextInt();  //save menu input
        }
        else{
         //error, no number was entered
          System.out.println("You did not enter a number, exiting now...");
          menuinput = 5;  //set menu option to exit cycle
        }
      
        if(menuinput > 5 || menuinput < 1)
        {
        	//number input was outside of range
    	    System.out.println("You did not enter a valid number, exiting now...");
            menuinput = 5;  //set menu option to exit cycle
        }
      
        //print entire database
        if(menuinput == 1)
        {
    	    System.out.println("Creating statement...");
            stmt = conn.createStatement();
            String sql;
            sql = "SELECT * FROM books";
            ResultSet rs = stmt.executeQuery(sql);

            //print whole database
            while(rs.next()){
     	   //Retrieve by column name
                int id  = rs.getInt("id");
                String title = rs.getString("title");
                String author = rs.getString("author");
                int price = rs.getInt("price");
           
             //Display values
               System.out.print("ID: " + id);
               System.out.print(", Title: " + title);
               System.out.print(", Author: " + author);
               System.out.println(", Price: " + price);
            }          
            rs.close();
            stmt.close(); 
        }
        //insert a book into the database
        else if(menuinput == 2){
    	    System.out.println("What is the title the book you want to insert?");
    	    scanner.nextLine();
    	    String booktitle = scanner.nextLine();
    	    System.out.println("What is the author the book you want to insert?");
    	    String bookauthor = scanner.nextLine();
    	    System.out.println("What is the price the book you want to insert?");
    	    int bookprice = 0;
    	    //only except numbers for price
    	    if(scanner.hasNextInt()) {
    	    	bookprice = scanner.nextInt();
            }
            else{
              System.out.println("You did not enter a number, exiting now...");
              break;
            }
    	    System.out.println("Creating statement...");
            stmt = conn.createStatement();
            String sql;
            //get the value of the highest id in the database
            sql = "SELECT * FROM books WHERE id=( SELECT max(id) FROM books );";
            ResultSet rs = stmt.executeQuery(sql);
            int newid = 0;
            while(rs.next()){
          	   //Retrieve by column name
               newid  = rs.getInt("id");
            }
            newid = newid + 1;   //add one to the highest id before adding in the database
            
            System.out.println("Creating statement...");
            sql = "INSERT INTO books (id, title, author, price) VALUES ('" + newid + "', '" + booktitle + "', '" + bookauthor + "', '" + bookprice + "');";
            stmt.executeUpdate(sql);
          
    	    System.out.println("Creating statement...");
            sql = "SELECT * FROM books";
            rs = stmt.executeQuery(sql);

            //print whole database
            while(rs.next()){
     	   //Retrieve by column name
              int id  = rs.getInt("id");
              String title = rs.getString("title");
              String author = rs.getString("author");
              int price = rs.getInt("price");
           
             //Display values
              System.out.print("ID: " + id);
              System.out.print(", Title: " + title);
              System.out.print(", Author: " + author);
              System.out.println(", Price: " + price);
            }
            rs.close();
            stmt.close();
        }
        //delete from database by book title
        else if(menuinput == 3){
    	  System.out.println("What is the title the book you want to delete?");
    	  scanner.nextLine();
    	  String booktitle = scanner.nextLine();
    	  System.out.println("Creating statement...");
    	  stmt = conn.createStatement();
          String sql;
          sql = "DELETE FROM books WHERE title='" + booktitle + "';";
          stmt.executeUpdate(sql);
          
          System.out.println("Creating statement...");
          stmt = conn.createStatement();
          sql = "SELECT * FROM books";
          ResultSet rs = stmt.executeQuery(sql);

          //print whole database
          while(rs.next()){
     	   //Retrieve by column name
              int id  = rs.getInt("id");
              String title = rs.getString("title");
              String author = rs.getString("author");
              int price = rs.getInt("price");
           
             //Display values
             System.out.print("ID: " + id);
             System.out.print(", Title: " + title);
             System.out.print(", Author: " + author);
             System.out.println(", Price: " + price);
          }
          rs.close();
          stmt.close();
        }
        //search database by book title
        else if(menuinput == 4){
    	  System.out.println("What is the title the book you want to search for?");
    	  scanner.nextLine();
    	  String booktitle = scanner.nextLine();
    	  System.out.println("Creating statement...");
          stmt = conn.createStatement();
          String sql;
          sql = "SELECT * FROM books WHERE title='" + booktitle + "';";
          ResultSet rs = stmt.executeQuery(sql);

          //show data requested
          while(rs.next()){
     	   //Retrieve by column name
              int id  = rs.getInt("id");
              String title = rs.getString("title");
              String author = rs.getString("author");
              int price = rs.getInt("price");
           
             //Display values
             System.out.print("ID: " + id);
             System.out.print(", Title: " + title);
             System.out.print(", Author: " + author);
             System.out.println(", Price: " + price);
          }
          rs.close();
          stmt.close();
        }
        //exit cycle option
        else if(menuinput == 5)
        {
        	menuexit = 1;  //exit menu cycle
        }
      }
        conn.close();
   }catch(SQLException se){
      //Handle errors for JDBC
      se.printStackTrace();
   }catch(Exception e){
      //Handle errors for Class.forName
      e.printStackTrace();
   }finally{
      //finally block used to close resources
      try{
         if(stmt!=null)
            stmt.close();
      }catch(SQLException se2){
      }// nothing we can do
      try{
         if(conn!=null)
            conn.close();
      }catch(SQLException se){
         se.printStackTrace();
      }//end finally try
   }//end try
   System.out.println("Goodbye!");
}//end main
   
}