SELECT s.genere, a.nomeArte, v.stipendiomax
FROM Stile s
	JOIN Artista a ON s.artista = a.nomeArte
	JOIN Cliente cl ON a.nomeArte = cl.artista
	JOIN Contratto c ON cl.CF = c.cliente
	JOIN StipendioMaxPerGenere v ON s.genere = v.genere AND c.remunerazione = v.stipendiomax;
