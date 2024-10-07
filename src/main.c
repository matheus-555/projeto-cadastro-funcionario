#include <stdio.h>
#include <stdlib.h>
#include "../inc/funcionario.h"

// Modos de implementação de alocacao de memoria
#define ALOCACAO_DINAMICA 0
#define ALOCACAO_ESTATICA 1

// Definir o tipo de implementacao desejada
#define TIPO_DE_USO ALOCACAO_DINAMICA

#define QTDE_FUNCIONARIO 3

int main(int argc, char const *argv[])
{
#if TIPO_DE_USO == USANDO_ALOCACAO_DINAMICA
    funcionario_t **f = malloc(QTDE_FUNCIONARIO * sizeof(funcionario_t *));
    f[0] = funcionario_init_dinamico(FUNCIONARIO_CONTRATO_TERCEIRIZADO, (funcionario_tipo_contrato_t){.terceirizado = {.valor_por_hora = 50, .horas_trabalhadas = 100}}, "Joao", 0, 0);
    f[1] = funcionario_init_dinamico(FUNCIONARIO_CONTRATO_CLT, (funcionario_tipo_contrato_t){.clt = {.salario = 3000}}, "Marcela", 10, 200);
    f[2] = funcionario_init_dinamico(FUNCIONARIO_CONTRATO_TERCEIRIZADO, (funcionario_tipo_contrato_t){.terceirizado = {.valor_por_hora = 30, .horas_trabalhadas = 180}}, "Joaquim", 12, 200);
    funcionario_imprime_funcionarios_dinamico(f, QTDE_FUNCIONARIO);
    printf("Folha de pagamento: %.2lf\n", funcionario_calcular_folha_pagamento_dinamico(f, QTDE_FUNCIONARIO));
    funcionario_close_all(&f, QTDE_FUNCIONARIO);
#elif TIPO_DE_USO == ALOCACAO_ESTATICA
    funcionario_t f[QTDE_FUNCIONARIO];
    funcionario_init_estatico(&f[0], FUNCIONARIO_CONTRATO_TERCEIRIZADO, (funcionario_tipo_contrato_t){.terceirizado = {.valor_por_hora = 50, .horas_trabalhadas = 100}}, "Joao", 0, 0);
    funcionario_init_estatico(&f[1], FUNCIONARIO_CONTRATO_CLT, (funcionario_tipo_contrato_t){.clt = {.salario = 3000}}, "Marcela", 10, 200);
    funcionario_init_estatico(&f[2], FUNCIONARIO_CONTRATO_TERCEIRIZADO, (funcionario_tipo_contrato_t){.terceirizado = {.valor_por_hora = 30, .horas_trabalhadas = 180}}, "Joaquim", 12, 200);
    funcionario_imprime_funcionarios_estatico(&f[0], QTDE_FUNCIONARIO);
    printf("Folha de pagamento: %.2lf\n", funcionario_calcular_folha_pagamento_estatico(f, QTDE_FUNCIONARIO));
#endif

    printf("\n\nFinal Program\n\n");

    return EXIT_SUCCESS;
}
