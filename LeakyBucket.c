#include <stdio.h>

static int leak_amount(int bucket, int leak_rate) {
    return (bucket >= leak_rate) ? leak_rate : bucket;
}

void leaky_bucket(int bucket_capacity, int leak_rate, int num_packets, int packets[]) {
    int bucket = 0;

    printf("Time\tIncoming\tBucket\tDropped\tLeaked\tRemaining\n");
    for (int i = 0; i < num_packets; i++) {
        int dropped = 0;

        bucket += packets[i];
        if (bucket > bucket_capacity) {
            dropped = bucket - bucket_capacity;
            bucket = bucket_capacity;
        }

        int leaked = leak_amount(bucket, leak_rate);
        bucket -= leaked;

        printf("%d%10d%10d%10d%10d%10d\n", i + 1, packets[i], bucket + leaked, dropped, leaked, bucket);
    }

    int time = num_packets + 1;
    while (bucket > 0) {
        int leaked = leak_amount(bucket, leak_rate);
        printf("%d%10d%10d%10d%10d%10d\n", time, 0, bucket, 0, leaked, bucket - leaked);
        bucket -= leaked;
        time++;
    }
}

int main() {
    int bucket_capacity, leak_rate, num_packets;

    printf("Enter The Bucket Capacity: ");
    scanf("%d", &bucket_capacity);

    printf("Enter The Leak Rate: ");
    scanf("%d", &leak_rate);

    printf("Enter The Number of Packets: ");
    scanf("%d", &num_packets);

    int packets[num_packets];
    printf("Enter The Size of Each Incoming Packet:\n");
    for (int i = 0; i < num_packets; i++) {
        scanf("%d", &packets[i]);
    }

    printf("\nLeaky Bucket Simulation:\n");
    leaky_bucket(bucket_capacity, leak_rate, num_packets, packets);

    return 0;
}

