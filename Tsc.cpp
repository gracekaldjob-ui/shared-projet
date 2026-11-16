/*
 * Practical Work 1: Pizza Pour Tous (PPT)
 * C code in English | Terminal display in French
 */
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define RESTAURANT_NAME    "Pizza Pour Tous"
#define RESTAURANT_ADDRESS "Keyce Titi garage"
#define RESTAURANT_PHONE   "+237 620 239 480"
#define GST_RATE           0.05
#define QST_RATE           0.09975
#define MEAL_COUNT         6
#define MAX_ITEMS          50

typedef struct { int id; const char *desc; double price; } Meal;
typedef struct { int mealIdx; char pizza; int qty; } OrderItem;

const Meal MEALS[MEAL_COUNT] = {
    {3,  "1 Pizza Moyenne 12\" + 1 Petit shawarma + 1 Pepsi (355 ml)",             20.49},
    {4,  "1 Pizza Moyenne 12\" + 10 Ailes de poulet + Frites + 1 Pepsi (355 ml)", 21.99},
    {8,  "1 Grande Pizza 14\" + 1 Shawarma Moyen + 1 Pepsi (355 ml)",             24.49},
    {9,  "1 Grande Pizza 14\" + 12 Ailes de poulet + Frites + 1 Pepsi (355 ml)",  25.99},
    {13, "1 Tres Grande Pizza 16\" + 1 Grand Shawarma + 1 Pepsi (355 ml)",        28.49},
    {14, "1 Tres Grande Pizza 16\" + 15 Ailes de poulet + 1 Pepsi (355 ml)",      29.99}
};

void clearLine(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

void readLine(char *buf, int size) {
    if (fgets(buf, size, stdin) != NULL) {
        buf[strcspn(buf, "\r\n")] = '\0';
    }
}

void formatNow(char *buf, size_t size) {
    time_t t = time(NULL);
    struct tm *tmInfo = localtime(&t);
    if (tmInfo != NULL) {
        strftime(buf, size, "%Y-%m-%d %H:%M:%S", tmInfo);
    }
}

int findMeal(int id) {
    int i;
    for (i = 0; i < MEAL_COUNT; i++) {
        if (MEALS[i].id == id) return i;
    }
    return -1;
}

const char *pizzaLabel(char c) {
    switch (tolower((unsigned char)c)) {
        case 'v': return "Vegetarienne";
        case 'f': return "Fromage";
        case 't': return "Toute garnie";
        case 'e': return "Epinards";
        case 'm': return "Fruits de mer";
        default:  return NULL;
    }
}

const char *paymentLabel(char c) {
    switch (toupper((unsigned char)c)) {
        case 'C': return "Comptant";
        case 'D': return "Debit";
        case 'R': return "Credit";
        default:  return NULL;
    }
}

void billOrder(int *invoiceNo, int *totalMeals, double *totalSales) {
    char phone[30], address[120], dateTime[32], more = 'n', pay;
    OrderItem items[MAX_ITEMS];
    int count = 0, i, orderMeals = 0;
    double sub = 0.0, gst, qst, total;

    clearLine();
    printf("\nEntrez le numero de telephone du client : ");
    readLine(phone, sizeof(phone));
    printf("Entrez l'adresse du client : ");
    readLine(address, sizeof(address));

    do {
        int mealNum, mealIdx, qty;
        char pizza;

        printf("\n--- TABLE DES REPAS (SUPER COMBOS) ---\n");
        for (i = 0; i < MEAL_COUNT; i++) {
            printf("No %-2d : %s - %.2f $\n", MEALS[i].id, MEALS[i].desc, MEALS[i].price);
        }
        printf("--------------------------------------\n");

        printf("Entrez le numero du repas (3, 4, 8, 9, 13 ou 14) : ");
        scanf("%d", &mealNum);
        while ((mealIdx = findMeal(mealNum)) == -1) {
            printf("Erreur : numero de repas invalide. Choisissez 3, 4, 8, 9, 13 ou 14 : ");
            scanf("%d", &mealNum);
        }

        printf("Type de pizza (v=Vegetarienne, f=Fromage, t=Toute garnie, e=Epinards, m=Fruits de mer) : ");
        scanf(" %c", &pizza);
        while (pizzaLabel(pizza) == NULL) {
            printf("Erreur : type de pizza invalide. Entrez v, f, t, e ou m : ");
            scanf(" %c", &pizza);
        }

        printf("Entrez le nombre de repas (> 0) : ");
        scanf("%d", &qty);
        while (qty <= 0) {
            printf("Erreur : la quantite doit etre strictement superieure a 0 : ");
            scanf("%d", &qty);
        }

        if (count < MAX_ITEMS) {
            items[count].mealIdx = mealIdx;
            items[count].pizza = pizza;
            items[count].qty = qty;
            count++;
        }

        printf("Voulez-vous ajouter un autre repas ? (o/O = Oui, n/N = Non) : ");
        scanf(" %c", &more);
        while (tolower((unsigned char)more) != 'o' && tolower((unsigned char)more) != 'n') {
            printf("Erreur : reponse invalide. Entrez o/O (Oui) ou n/N (Non) : ");
            scanf(" %c", &more);
        }
    } while (tolower((unsigned char)more) == 'o' && count < MAX_ITEMS);

    printf("Mode de paiement (C = Comptant, D = Debit, R = Credit) : ");
    scanf(" %c", &pay);
    while (paymentLabel(pay) == NULL) {
        printf("Erreur : mode de paiement invalide. Entrez C, D ou R : ");
        scanf(" %c", &pay);
    }

    for (i = 0; i < count; i++) {
        sub += MEALS[items[i].mealIdx].price * items[i].qty;
        orderMeals += items[i].qty;
    }
    gst = sub * GST_RATE;
    qst = sub * QST_RATE;
    total = sub + gst + qst;
    formatNow(dateTime, sizeof(dateTime));
    *totalMeals += orderMeals;
    *totalSales += total;

    printf("\n================================================================\n");
    printf("                    %s\n", RESTAURANT_NAME);
    printf("           Adresse : %s\n", RESTAURANT_ADDRESS);
    printf("           Telephone : %s\n", RESTAURANT_PHONE);
    printf("================================================================\n");
    printf("Facture No       : %d\nDate et heure    : %s\n", (*invoiceNo)++, dateTime);
    printf("Adresse client   : %s\nTelephone client : %s\n", address, phone);
    printf("Mode de paiement : %s\n----------------------------------------------------------------\n", paymentLabel(pay));
    printf("Repas commandes :\n");
    for (i = 0; i < count; i++) {
        Meal m = MEALS[items[i].mealIdx];
        printf("  %d) [No %d] %s\n", i + 1, m.id, m.desc);
        printf("     Pizza : %s | Qte : %d | Prix unit. : %.2f $ | Total : %.2f $\n",
               pizzaLabel(items[i].pizza), items[i].qty, m.price, m.price * items[i].qty);
    }
    printf("----------------------------------------------------------------\n");
    printf("Sous-total       : %8.2f $\nTPS (5.00 %%)     : %8.2f $\n", sub, gst);
    printf("TVQ (9.975 %%)    : %8.2f $\nTOTAL A PAYER    : %8.2f $\n", qst, total);
    printf("================================================================\n");
    printf("Merci d'avoir commande chez %s ! Bon appetit !\n", RESTAURANT_NAME);
    printf("================================================================\n\n");
}

int main(void) {
    int choice, invoiceNo = 1, totalMeals = 0;
    double totalSales = 0.0;
    char dateTime[32];

    setvbuf(stdout, NULL, _IONBF, 0);

    printf("================================================================\n");
    printf("       BIENVENUE CHEZ %s (PPT)\n", RESTAURANT_NAME);
    printf("================================================================\n");
    printf("Ce programme permet de gerer la prise de commandes par telephone,\n");
    printf("de calculer et d'afficher les factures avec taxes (TPS et TVQ),\n");
    printf("et de suivre le total des repas vendus et des ventes.\n");
    printf("================================================================\n\n");

    do {
        printf("--- MENU PRINCIPAL ---\n1. Facturer une commande\n");
        printf("2. Afficher le nombre total de repas vendus et le montant total des ventes\n");
        printf("3. Quitter le programme\nVotre choix (1, 2 ou 3) : ");
        scanf("%d", &choice);
        while (choice < 1 || choice > 3) {
            printf("Erreur : option invalide. Veuillez entrer 1, 2 ou 3 : ");
            scanf("%d", &choice);
        }

        switch (choice) {
            case 1:
                billOrder(&invoiceNo, &totalMeals, &totalSales);
                break;
            case 2:
                formatNow(dateTime, sizeof(dateTime));
                printf("\n================================================================\n");
                printf("                RAPPORT DES VENTES - %s\n", RESTAURANT_NAME);
                printf("================================================================\n");
                printf("Entreprise             : %s\nTelephone              : %s\n", RESTAURANT_NAME, RESTAURANT_PHONE);
                printf("Adresse                : %s\nDate et heure actuelles: %s\n", RESTAURANT_ADDRESS, dateTime);
                printf("----------------------------------------------------------------\n");
                printf("Nombre total de repas vendus : %d\nMontant total des ventes     : %.2f $\n", totalMeals, totalSales);
                printf("================================================================\n\n");
                break;
            case 3:
                printf("\nMerci et a la prochaine !\n");
                break;
        }
    } while (choice != 3);
    return 0;
}

