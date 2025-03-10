#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dependencies/include/libpq-fe.h"

# define PG_HOST "localhost"
# define PG_USER "postgres" // nome utente
# define PG_DB "DBrecords" // nome database
# define PG_PASS "" // password
# define PG_PORT 5432

void checkResults(PGresult *res, const PGconn *conn) {
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("Risultati inconsistenti: %s\n", PQerrorMessage(conn));
        PQclear(res);
        exit(1);
    }
}

// Funzione per formattare correttamente la visualizzazione delle varie query nel prompt
void printFormattedTable(PGresult *queryResult) {
    int numFields = PQnfields(queryResult);
    int numRows = PQntuples(queryResult);

    // Trova la massima larghezza per ogni colonna
    int maxColWidth[numFields];
    for (int i = 0; i < numFields; i++) {
        maxColWidth[i] = strlen(PQfname(queryResult, i));
        for (int j = 0; j < numRows; j++) {
            int width = strlen(PQgetvalue(queryResult, j, i));
            if (width > maxColWidth[i]) {
                maxColWidth[i] = width;
            }
        }
    }

    // Stampa nomi delle colonne
    for (int i = 0; i < numFields; i++) {
        printf("%-*s\t", maxColWidth[i], PQfname(queryResult, i));
    }
    printf("\n");

    // Stampa righe
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numFields; j++) {
            printf("%-*s\t", maxColWidth[j], PQgetvalue(queryResult, i, j));
        }
        printf("\n");
    }
}

void executeAndPrintQuery(PGconn *conn, const char *query) {
    PGresult *res = PQexec(conn, query);
    checkResults(res, conn);
    printFormattedTable(res);
    PQclear(res);
}

int main(int argc, char **argv) {
    char conninfo[250];
    sprintf(conninfo, "user=%s password=%s dbname=%s host=%s port=%d", PG_USER, PG_PASS, PG_DB, PG_HOST, PG_PORT);

    PGconn *conn = PQconnectdb(conninfo);
    // Verifico lo stato di connessione
    if (PQstatus(conn) != CONNECTION_OK) {
        printf("Errore di connessione: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
    printf("Connessione avvenuta correttamente\n");

    int choice;
    while (1) {
        printf("Seleziona una query da eseguire e stampare:\n");
        printf("1. Informazioni sugli artisti con almeno una certificazione\n");
        printf("2. Numero di strumentisti per strumento\n");
        printf("3. Incisione canzoni/album negli studi di registrazione\n");
        printf("4. Artisti (non Band) che hanno suonato in palazzetti (non in stadi)\n");
        printf("5. Artisti con il massimo stipendio per genere\n");
        printf("6. Esci\n");
        printf("Scelta: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                executeAndPrintQuery(conn, "SELECT a.nomeArte AS Artista, COUNT(DISTINCT c.id) AS NumeroCanzoni, COUNT(DISTINCT al.id) AS NumeroAlbum, SUM(c.numAscolti) AS TotaleAscoltiCanzoni, SUM(al.numAscolti) AS TotaleAscoltiAlbum, COUNT(DISTINCT pc.certificazione) AS NumeroCertificazioniCanzoni, COUNT(DISTINCT pa.certificazione) AS NumeroCertificazioniAlbum FROM Artista a LEFT JOIN Canzone c ON a.nomeArte = c.artista LEFT JOIN Album al ON a.nomeArte = al.artista LEFT JOIN PremioCanzone pc ON c.id = pc.canzone LEFT JOIN PremioAlbum pa ON al.id = pa.album GROUP BY a.nomeArte HAVING COUNT(DISTINCT pc.certificazione) > 0 OR COUNT(DISTINCT pa.certificazione) > 0 ORDER BY a.nomeArte;");
                break;
            case 2:
                executeAndPrintQuery(conn, "SELECT st.nome AS Strumento, COUNT(u.strumentista) AS NumeroStrumentisti FROM Utilizzo u JOIN Strumento st ON u.strumento = st.nome GROUP BY st.nome;");
                break;
            case 3:
                executeAndPrintQuery(conn, "SELECT st.città, st.indirizzo, COUNT(DISTINCT al.id) AS NumeroAlbum, COUNT(DISTINCT c.id) AS NumeroCanzoni FROM StudioRegistrazione st JOIN Album al ON al.cittàReg = st.città AND al.indReg = st.indirizzo JOIN Canzone c ON c.cittàReg = st.città AND c.indReg = st.indirizzo GROUP BY st.città, st.indirizzo;");
                break;
            case 4:
                executeAndPrintQuery(conn, "SELECT a.nomeArte AS NomeArte, c.nome AS NomeReale, c.cognome AS CognomeReale FROM Artista a JOIN Cliente c ON a.nomeArte = c.artista LEFT JOIN Band b ON a.nomeArte = b.artista WHERE b.artista IS NULL AND a.nomeArte IN (SELECT DISTINCT co.artista FROM Concerto co WHERE co.luogo = 'palazzetto') AND a.nomeArte NOT IN (SELECT DISTINCT co.artista FROM Concerto co WHERE co.luogo = 'stadio');");
                break;
            case 5:
                executeAndPrintQuery(conn, "SELECT s.genere, a.nomeArte, v.stipendiomax FROM Stile s JOIN Artista a ON s.artista = a.nomeArte JOIN Cliente cl ON a.nomeArte = cl.artista JOIN Contratto c ON cl.CF = c.cliente JOIN StipendioMaxPerGenere v ON s.genere = v.genere AND c.remunerazione = v.stipendiomax;");
                break;
            case 6:
                PQfinish(conn);
                return 0;
            default:
                printf("Scelta non valida. Riprova.\n");
        }
        printf("-----------\n");
    }

    return 0;
}
