SELECT st.nome AS Strumento, COUNT(u.strumentista) AS NumeroStrumentisti
FROM Utilizzo u
JOIN Strumento st ON u.strumento = st.nome
GROUP BY st.nome;
