// Code for Quick Tests

#include <libpq-fe.h>
#include <iostream>

int main() {
  // Connect to the database
  PGconn *conn = PQconnectdb("host=localhost user=postgres password=password dbname=mydb");

  // Check if the connection was successful
  if (PQstatus(conn) != CONNECTION_OK) {
    // If the connection failed, throw an exception
    throw std::runtime_error(PQerrorMessage(conn));
  }

  // Execute a query
  PGresult *res = PQexec(conn, "SELECT * FROM users");

  // Check if the query was successful
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    // If the query failed, throw an exception
    throw std::runtime_error(PQresultErrorMessage(res));
  }

  // Process the result set
  // ...

  // Clean up
  PQclear(res);
  PQfinish(conn);

  return 0;
}