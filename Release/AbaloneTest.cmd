echo Monte Carlo vs Random Moves
for /L %%i IN (1 1 10) do Abalone.exe -Batch -NoOfTurns=500 -Formation=Standard -Player1=MC -SimGames1=100 -TurnsPerSim1=100 -Evaluation1=1 -Player2=RM
for /L %%i IN (1 1 10) do Abalone.exe -Batch -NoOfTurns=500 -Formation=Standard -Player1=MC -SimGames1=200 -TurnsPerSim1=100 -Evaluation1=1 -Player2=RM
for /L %%i IN (1 1 10) do Abalone.exe -Batch -NoOfTurns=500 -Formation=Standard -Player1=MC -SimGames1=300 -TurnsPerSim1=100 -Evaluation1=1 -Player2=RM


echo Alpha Beta vs Random Moves
for /L %%i IN (1 1 10) do Abalone.exe -Batch -NoOfTurns=500 -Formation=Standard -Player1=AB -TreeDepth1=4 -Evaluation1=1 -Player2=RM
for /L %%i IN (1 1 10) do Abalone.exe -Batch -NoOfTurns=500 -Formation=Standard -Player1=AB -TreeDepth1=5 -Evaluation1=1 -Player2=RM
for /L %%i IN (1 1 10) do Abalone.exe -Batch -NoOfTurns=500 -Formation=Standard -Player1=AB -TreeDepth1=6 -Evaluation1=1 -Player2=RM
