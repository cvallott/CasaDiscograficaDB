SELECT a.nomeArte AS Artista, COUNT(DISTINCT c.id) AS NumeroCanzoni, 
	COUNT(DISTINCT al.id) AS NumeroAlbum,
    SUM(c.numAscolti) AS TotaleAscoltiCanzoni, 
	SUM(al.numAscolti) AS TotaleAscoltiAlbum,
	COUNT(DISTINCT pc.certificazione) AS NumeroCertificazioniCanzoni, 
	COUNT(DISTINCT pa.certificazione) AS NumeroCertificazioniAlbum
FROM Artista a LEFT JOIN Canzone c ON a.nomeArte = c.artista
	LEFT JOIN Album al ON a.nomeArte = al.artista
	LEFT JOIN PremioCanzone pc ON c.id = pc.canzone
	LEFT JOIN PremioAlbum pa ON al.id = pa.album
GROUP BY a.nomeArte
HAVING COUNT(DISTINCT pc.certificazione) > 0 OR COUNT(DISTINCT pa.certificazione) > 0
ORDER BY a.nomeArte;


