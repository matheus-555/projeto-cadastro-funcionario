#ifndef __FUNCIONARIO_H__
#define __FUNCIONARIO_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// --- Atributos publicos

// HEURISTICA PARA AUMENTAR O GRAU DE SEGURANÇA DA VERIFICACAO DO TIPO DA ESTRUTURA DE CONTRATO EM FUNCAO DO TIPO DE CONTRATO DEFINIDO
#define FUNCIONARIO_MIN_VALOR_HORA 0.01
#define FUNCIONARIO_MIN_SALARIO 0.01

// Tamanho maximo para o buffer do nome
#define FUNCIONARIO_LEN_NOME (25 + 1)

// Status de retorno dos metodos dessa estrutura
typedef enum
{
    FUNCIONARIO_RETORNO_OK = 0,
    FUNCIONARIO_RETORNO_FAIL,
    FUNCIONARIO_RETORNO_LEN
} funcionario_retorno_typedef;

// Tipos de contrato
typedef enum
{
    FUNCIONARIO_CONTRATO_CLT = 0,
    FUNCIONARIO_CONTRATO_TERCEIRIZADO,
    FUNCIONARIO_CONTRATO_LEN
} funcionario_contrato_typedef;

typedef struct
{
    double salario;
} funcionario_clt_t;

typedef struct
{
    uint32_t horas_trabalhadas;
    double valor_por_hora;
} funcionario_terceirizado_t;

typedef union
{
    funcionario_clt_t clt;
    funcionario_terceirizado_t terceirizado;
} funcionario_tipo_contrato_t;

typedef struct
{
    funcionario_contrato_typedef tipo_contrato;
    funcionario_tipo_contrato_t contrato;
    char nome[FUNCIONARIO_LEN_NOME];
    uint32_t contratos_assinados;
    double comissao_por_contrato;
} funcionario_t;

// Atributos Publicos
extern const char *FUNCIONARIO_TIPO_CONTRATO[FUNCIONARIO_CONTRATO_LEN];

// --- Metodos publicos
funcionario_t *funcionario_init_dinamico(funcionario_contrato_typedef tipo_contrato, funcionario_tipo_contrato_t contrato, char *nome, uint32_t contratos_assinados, double comissao_por_contrato);
funcionario_retorno_typedef funcionario_init_estatico(funcionario_t *obj, funcionario_contrato_typedef tipo_contrato, funcionario_tipo_contrato_t contrato, char *nome, uint32_t contratos_assinados, double comissao_por_contrato);
funcionario_retorno_typedef funcionario_close(funcionario_t **obj);
funcionario_retorno_typedef funcionario_close_all(funcionario_t ***obj, uint32_t length);
double funcionario_calcular_total_pagar_funcionario(funcionario_t *obj);
void funcionario_imprime_funcionarios_dinamico(funcionario_t **obj, uint32_t lenght);
void funcionario_imprime_funcionarios_estatico(funcionario_t *obj, uint32_t lenght);
double funcionario_calcular_folha_pagamento_dinamico(funcionario_t **obj, uint32_t lenght);
double funcionario_calcular_folha_pagamento_estatico(funcionario_t *obj, uint32_t lenght);

#endif