#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define binaryPartSize 8
#define maxIPToProcess 50

int usedIPStack = 0;

typedef struct {
    int one;
    int two;
    int three;
    int four;
} IP;

typedef struct {
    int one[8];
    int two[8];
    int three[8];
    int four[8];
} IPBinary;

typedef enum {
    firstIP,
    lastIP,
    hosts,
    broadcast
} name;

int decToBinary(int n, int arr[]) {
    int binaryNum[binaryPartSize];
    int num = 0;
    // counter for binary array
    int i = 0;
    while (n > 0) {
        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }

    int helper = 1;
    for (int j = 0; j < i; j++) {
        num += binaryNum[j] * helper;
        helper = helper * 10;
    }
    for (int j = 0; j < i; j++) {
        arr[j] = binaryNum[j];
    }
    return num;
}

IP ipToBin(IP ip, IPBinary *ipBin) {
    ip.one = decToBinary(ip.one, ipBin->one);
    ip.two = decToBinary(ip.two, ipBin->two);
    ip.three = decToBinary(ip.three, ipBin->three);
    ip.four = decToBinary(ip.four, ipBin->four);

    return ip;
}

void verknüpfen(int arr[], int arr2[], int result[]) {
    for (int i = 0; i < binaryPartSize; i++) {
        if (arr[i] == 1 && arr2[i] == 1) {
            result[i] = 1;
        } else {
            result[i] = 0;
        }
    }
}

int changeBinFormat(int arr[]) {
    int num = 0;
    int helper = 1;

    for (int j = 0; j < binaryPartSize; j++) {
        num += arr[j] * helper;
        helper = helper * 10;
    }

    return num;
}

IP logischverknupfen(IP ip, IP subnet, IPBinary *binResult) {
    IPBinary ipBin;
    IPBinary subBin;
    IPBinary result;
    IP returnResult;

    ipToBin(ip, &ipBin);
    ipToBin(subnet, &subBin);


    verknüpfen(ipBin.one, subBin.one, result.one);
    verknüpfen(ipBin.two, subBin.two, result.two);
    verknüpfen(ipBin.three, subBin.three, result.three);
    verknüpfen(ipBin.four, subBin.four, result.four);

    returnResult.one = changeBinFormat(result.one);
    returnResult.two = changeBinFormat(result.two);
    returnResult.three = changeBinFormat(result.three);
    returnResult.four = changeBinFormat(result.four);

    *binResult = result;

    return returnResult;
}

int binToDec(int arr[]) {
    int num = 0;
    int helper = 1;
    for (int i = 0; i < binaryPartSize; i++) {
        num += arr[i] * helper;
        helper = helper * 2;
    }
    return num;
}

IP ipBinToDec(IPBinary ipBin) {
    IP ip;
    ip.one = binToDec(ipBin.one);
    ip.two = binToDec(ipBin.two);
    ip.three = binToDec(ipBin.three);
    ip.four = binToDec(ipBin.four);

    return ip;
}

void read(IP ip[maxIPToProcess], int *subnet) {
    for (int i = 0; i < maxIPToProcess; i++) {
        printf("Enter IP or 0 to quit: ");
        scanf("%d.%d.%d.%d", &ip[i].one, &ip[i].two, &ip[i].three, &ip[i].four);
        usedIPStack++;
        if (ip[i].one == 0) {
            i = maxIPToProcess + 1;
            usedIPStack--;
            goto exit;
        }
        printf("Enter Subnetmask : ");
        scanf("%d", subnet);
    }
exit:

}

void printIP(IP ip[], IP subnetMask[maxIPToProcess], int subnetmask[]) {
    for (int i = 0; i < usedIPStack; i++) {
        IPBinary ipBin;
        IP binary = ipToBin(ip[i], &ipBin);
        IP subnet = logischverknupfen(ip[i], subnetMask[i], &ipBin);
        IP subnetDec = ipBinToDec(ipBin);

        printf("-----------------------------------------------------------\n");
        printf("Original IP:   %d.%d.%d.%d/%d\n", ip[i].one, ip[i].two, ip[i].three, ip[i].four, subnetmask[i]);
        printf("Binary IP:     %d.%d.%d.%d\n", binary.one, binary.two, binary.three, binary.four);
        printf("Subnet:        %d.%d.%d.%d\n", subnetDec.one, subnetDec.two, subnetDec.three, subnetDec.four);
        printf("Subnet Binary: %d.%d.%d.%d\n", subnet.one, subnet.two, subnet.three, subnet.four);
    }
}

void zeroArray(IP ip[]) {
    for (int i = 0; i < maxIPToProcess; i++) {
        ip[i].one = 0;
        ip[i].two = 0;
        ip[i].three = 0;
        ip[i].four = 0;
    }
}

void calculateSubnets(IP subnet[maxIPToProcess], const int subnetMask[]) {
    for (int j = 0; j < usedIPStack; j++) {
        int subnetTry[32] = {0};
        for (int i = 0; i < subnetMask[j]; i++) {
            subnetTry[i] = 1;
        }

        int one[8] = {0}, two[8] = {0}, three[8] = {0}, four[8] = {0};

        for (int i = 0; i < 32; i++) {
            if (i < 8) {
                one[i] = subnetTry[i];
            } else if (i < 16) {
                two[i - 8] = subnetTry[i];
            } else if (i < 24) {
                three[i - 16] = subnetTry[i];
            } else if (i < 32) {
                four[i - 24] = subnetTry[i];
            }
        }

        subnet[j].one = binToDec(one);
        subnet[j].two = binToDec(two);
        subnet[j].three = binToDec(three);
        subnet[j].four = binToDec(four);
    }
}

void header() {
    printf("##############################\n");
    printf("#     Subnet Calculator      #\n");
    printf("#     Made by WildEpic       #\n");
    printf("##############################\n");

    printf("\n");
}

int main(void) {
    IP ip[maxIPToProcess];
    IP subnet[maxIPToProcess];
    int subnetMask[maxIPToProcess] = {0};

    header();

    zeroArray(ip);
    zeroArray(subnet);
    read(ip, subnetMask);
    calculateSubnets(subnet, subnetMask);

    system("clear");

    printIP(ip, subnet, subnetMask);


    return 0;
}
