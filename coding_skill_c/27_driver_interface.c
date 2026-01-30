/* Driver Interface Pattern
 * 드라이버 인터페이스 표준화
 */

#include <stdio.h>
#include <stdint.h>

typedef struct Driver {
    int (*open)(struct Driver* self);
    int (*close)(struct Driver* self);
    int (*read)(struct Driver* self, uint8_t* buf, size_t len);
    int (*write)(struct Driver* self, const uint8_t* buf, size_t len);
    void* private_data;
} Driver;

// UART 드라이버 구현
typedef struct {
    int port;
    int baud;
} UART_Private;

int UART_Open(Driver* self) {
    UART_Private* priv = (UART_Private*)self->private_data;
    printf("[UART%d] Open (baud: %d)\n", priv->port, priv->baud);
    return 0;
}

int UART_Close(Driver* self) {
    UART_Private* priv = (UART_Private*)self->private_data;
    printf("[UART%d] Close\n", priv->port);
    return 0;
}

int UART_Read(Driver* self, uint8_t* buf, size_t len) {
    printf("[UART] Read %zu bytes\n", len);
    return (int)len;
}

int UART_Write(Driver* self, const uint8_t* buf, size_t len) {
    printf("[UART] Write %zu bytes\n", len);
    return (int)len;
}

Driver* UART_Create(int port, int baud) {
    Driver* drv = malloc(sizeof(Driver));
    UART_Private* priv = malloc(sizeof(UART_Private));
    
    priv->port = port;
    priv->baud = baud;
    
    drv->open = UART_Open;
    drv->close = UART_Close;
    drv->read = UART_Read;
    drv->write = UART_Write;
    drv->private_data = priv;
    
    return drv;
}

int main(void) {
    printf("=== Driver Interface Pattern ===\n\n");
    
    Driver* uart = UART_Create(1, 115200);
    
    uart->open(uart);
    
    uint8_t buf[10];
    uart->write(uart, buf, 10);
    uart->read(uart, buf, 10);
    
    uart->close(uart);
    
    free(uart->private_data);
    free(uart);
    
    printf("\nDriver Interface: 드라이버 표준화 패턴\n");
    return 0;
}
