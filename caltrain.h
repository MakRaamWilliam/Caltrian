#include <pthread.h>

struct station {
	// FILL ME IN
	pthread_mutex_t mutex;

	int pass_wait;     // #passengers wait trians
	int pass_on_trian; // #passengers on trian
	int seat_empty;

	pthread_cond_t trian_arrive;  //to infrom wait passengrs
	pthread_cond_t pass_seat;   //to inform load trian
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
