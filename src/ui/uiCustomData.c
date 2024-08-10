#include <customDataTypes.h>
#include <gtk/gtk.h>

typedef struct {
  GtkWidget *label;
  GtkWidget *button;
} NetworkStatusData;

typedef struct {
  GtkWidget *v_box;
  struct ssdpMessage result;
  GtkWidget *spinner;
  GtkWidget *label_horizontal_box;
  GtkWidget *spin_label;
} UiUpdateData;

typedef struct {
  GtkWidget *window;
  guint timeout_id;
  NetworkStatusData *free_data;
} StartData;

typedef struct {
  char *myIp;
  double timer;
} JoinSSDPThread;
