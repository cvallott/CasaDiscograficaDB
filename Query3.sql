SELECT st.città, st.indirizzo, 
	COUNT(DISTINCT al.id) AS NumeroAlbum,
    COUNT(DISTINCT c.id) AS NumeroCanzoni
FROM StudioRegistrazione st 
	JOIN Album al ON al.cittàReg = st.città AND al.indReg = st.indirizzo
	JOIN Canzone c ON c.cittàReg = st.città AND c.indReg = st.indirizzo
GROUP BY st.città, st.indirizzo;

