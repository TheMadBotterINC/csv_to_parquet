#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

static void sql_escape_single_quotes(const char *in, char *out, size_t out_sz)
{
    // doubles single-quotes for SQL string literals
    size_t j = 0;
    for (size_t i = 0; in[i] && j + 2 < out_sz; i++)
    {
        if (in[i] == '\'')
        {
            if (j + 2 >= out_sz)
                break;
            out[j++] = '\'';
            out[j++] = '\'';
        }
        else
        {
            out[j++] = in[i];
        }
    }
    out[j] = '\0';
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s input.csv output.parquet\n", argv[0]);
        return 1;
    }
    const char *in_csv = argv[1];
    const char *out_pq = argv[2];

    // Escape paths for SQL literal safety
    char in_esc[2048], out_esc[2048];
    sql_escape_single_quotes(in_csv, in_esc, sizeof(in_esc));
    sql_escape_single_quotes(out_pq, out_esc, sizeof(out_esc));

    duckdb_database db;
    duckdb_connection con;

    if (duckdb_open(NULL, &db) != DuckDBSuccess)
    {
        fprintf(stderr, "Failed to open DuckDB\n");
        return 1;
    }
    if (duckdb_connect(db, &con) != DuckDBSuccess)
    {
        fprintf(stderr, "Failed to connect\n");
        duckdb_close(&db);
        return 1;
    }

    // If you ever want all columns as VARCHAR (safer schema), add: , ALL_VARCHAR=TRUE
    char sql[4096];
    snprintf(sql, sizeof(sql),
             "COPY (SELECT * FROM read_csv_auto('%s', HEADER=TRUE)) "
             "TO '%s' (FORMAT PARQUET);",
             in_esc, out_esc);

    duckdb_result res;
    if (duckdb_query(con, sql, &res) != DuckDBSuccess)
    {
        const char *err = duckdb_result_error(&res);
        fprintf(stderr, "Query failed%s%s\n",
                err && *err ? ": " : "",
                err && *err ? err : "");
        duckdb_destroy_result(&res);
        duckdb_disconnect(&con);
        duckdb_close(&db);
        return 1;
    }

    duckdb_destroy_result(&res);
    duckdb_disconnect(&con);
    duckdb_close(&db);

    printf("Wrote Parquet: %s\n", out_pq);
    return 0;
}
