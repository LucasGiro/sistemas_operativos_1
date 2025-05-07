//inicializar el semaforo en 0
//llegoA;
//llegoB;

// pagina 12, chapter 13 del the book of semaphores
// problema 3.3

// 2 semaforos en 0;


//thread A;
//a1;
//a2;

//Thread B;
//b1;
//b2;

//a1 pase antes que b2 y que b1 pase antes que a2;

//solucion

//thread A;
//a1;
//llegoA.post()
//llegaB.wait()
//a2;

//Thread B;
//b1;
//llegoB.post()
//llegaA.wait()
//b2;

//a1 pase antes que b2 y que b1 pase antes que a2;

// para el barbero, usar cola concurrente de variables de condicion o semáforos.
// para la cola de semáforos, todos los semaforos en 0 e ir levantando el semaforo que va.
// usar una barrera para que no paguen antes de que cortando.
// o usar lo anterior para cortando-mecortan-pagando-mepagan