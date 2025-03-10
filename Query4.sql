SELECT a.nomeArte AS NomeArte, c.nome AS NomeReale, c.cognome AS CognomeReale
FROM Artista a JOIN Cliente c ON a.nomeArte = c.artista
LEFT JOIN Band b ON a.nomeArte = b.artista
WHERE b.artista IS NULL AND a.nomeArte IN (
	SELECT DISTINCT CO.artista
	FROM Concerto co
	WHERE co.luogo = 'palazzetto'
)
AND a.nomeArte NOT IN (
	SELECT DISTINCT co.artista
	FROM Concerto co
	WHERE co.luogo = 'stadio'
);