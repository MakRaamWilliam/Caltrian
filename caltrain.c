#include <pthread.h>
#include "caltrain.h"

/*which
will be invoked to initialize the station object when CalTrain boots in the file caltrain.c.
*/
void
station_init(struct station *station)
{
  // FILL ME IN

  pthread_mutex_init(&station->mutex,NULL);

  station->pass_on_trian=0;
  station->pass_wait=0;
  station->seat_empty=0;

  pthread_cond_init(&station->trian_arrive,NULL);
  pthread_cond_init(&station->pass_seat,NULL);

}

/*count indicates how many seats are available on the train. The function must not return
until the train is satisfactorily loaded (all passengers are in their seats, and either the train
is full or all waiting passengers have boarded). Note, that the train must leave the station
promptly if no passengers are waiting at the station or it has no available free seats.
*/

void
station_load_train(struct station *station, int count)
{
    // FILL ME IN

    station->seat_empty=count;  // to get acess in other function & not need critical sec
    pthread_mutex_lock(&station->mutex); //start critical sec all in it needed


    if(station->seat_empty == 0 || station->pass_wait == 0){ // no free seat or waiting so it return immd
      pthread_mutex_unlock(&station->mutex); // end of critical sec
      station->seat_empty=0;
    }else{
      pthread_cond_broadcast(&station->trian_arrive);         //use broadcast to inform all wait threads
      pthread_cond_wait(&station->pass_seat,&station->mutex); //wait signal from on board

      pthread_mutex_unlock(&station->mutex); // end of critical sec

      station->seat_empty=0;
    }
}

/*This function must not return until a train is in the station (i.e., a call to station load train is
in progress) and there are enough free seats on the train for this passenger to sit down. Once
this function returns, the passenger robot will move the passenger on board the train and into
a seat (you do not need to worry about how this mechanism works).
*/


void
station_wait_for_train(struct station *station)
{
    // FILL ME IN

    pthread_mutex_lock(&station->mutex); //start critcal sec

    station->pass_wait++;
    while( station->seat_empty == 0 )
      pthread_cond_wait(&station->trian_arrive,&station->mutex);

    station->pass_wait--;
    station->pass_on_trian++;
    station->seat_empty--;

    pthread_mutex_unlock(&station->mutex); // end critical sec
    //all func need to be in critical sec as it use only +/-
}


/*
Once the passenger is seated, it will call this function.
to let the train know that it’s on board.
*/

void
station_on_board(struct station *station)
{
   // FILL ME IN

   pthread_mutex_lock(&station->mutex); // start critcal sec

   station->pass_on_trian--;

   pthread_mutex_unlock(&station->mutex); // end critical sec

   if( station->pass_on_trian ==0 && (station->seat_empty ==0 || station->pass_wait == 0))
    pthread_cond_signal(&station->pass_seat);   // to inform load trian it finish board


}
