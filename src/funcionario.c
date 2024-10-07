#include "../inc/funcionario.h"

#define FUNCIONARIO_CARACTERE_DE_INVALIDO '\0'

// -------------------------------------------------------
// Atributos publicos
//--------------------------------------------------------
const char *FUNCIONARIO_TIPO_CONTRATO[FUNCIONARIO_CONTRATO_LEN] = {
    [FUNCIONARIO_CONTRATO_CLT]          = "CLT",
    [FUNCIONARIO_CONTRATO_TERCEIRIZADO] = "Terceirizado",
};

// -------------------------------------------------------
// Ponteiros de funcao private
// -------------------------------------------------------
typedef funcionario_retorno_typedef (*funcionario_set_contrato_t)(funcionario_t *, funcionario_tipo_contrato_t *);
typedef double (*funcionario_calcula_pagamento_t)(funcionario_t *);

// -------------------------------------------------------
// Metodos privados
// -------------------------------------------------------
static bool funcionario_is_valid(funcionario_t *obj);
static funcionario_retorno_typedef funcionario_set_invalid(funcionario_t *obj);

static funcionario_retorno_typedef funcionario_set_clt(funcionario_t *func_obj, funcionario_tipo_contrato_t *clt_obj);
static funcionario_retorno_typedef funcionario_set_terceirizado(funcionario_t *func_obj, funcionario_tipo_contrato_t *terc_obj);

static double funcionario_calcula_pagamento_clt(funcionario_t *obj);
static double funcionario_calcula_pagamento_terceirizado(funcionario_t *obj);

// -------------------------------------------------------
// Atribuico dos Callbacks
// -------------------------------------------------------
static funcionario_set_contrato_t funcionario_set_contrato_callback[FUNCIONARIO_CONTRATO_LEN] = {
    [FUNCIONARIO_CONTRATO_CLT]          = &funcionario_set_clt,
    [FUNCIONARIO_CONTRATO_TERCEIRIZADO] = &funcionario_set_terceirizado
};

static funcionario_calcula_pagamento_t funcionario_calcula_pagamento_callback[FUNCIONARIO_CONTRATO_LEN] = {
    [FUNCIONARIO_CONTRATO_CLT]          = &funcionario_calcula_pagamento_clt,
    [FUNCIONARIO_CONTRATO_TERCEIRIZADO] = &funcionario_calcula_pagamento_terceirizado
};

// -------------------------------------------------------
// Implementacao dos metodos publicos
// -------------------------------------------------------
funcionario_t *funcionario_init_dinamico(funcionario_contrato_typedef tipo_contrato, funcionario_tipo_contrato_t contrato, char *nome, uint32_t contratos_assinados, double comissao_por_contrato)
{
    funcionario_t *ret = NULL;

    // Verifica se a qtde de bytes - incluindo o '\0' - para o nome nao da overflow no buffer
    if ((strlen((const char *)nome) + 1) <= FUNCIONARIO_LEN_NOME)
    {
        ret = (funcionario_t *)calloc(1, sizeof(funcionario_t));

        if (ret != NULL)
        {
            ret->tipo_contrato = tipo_contrato;
            ret->contratos_assinados = contratos_assinados;
            ret->comissao_por_contrato = comissao_por_contrato;

            // Realiza as atribuicoes de acordo com o tipo de contrato  e verifica se a rotina retornou falha
            if (funcionario_set_contrato_callback[tipo_contrato](ret, &contrato) == FUNCIONARIO_RETORNO_OK)
            {
                strcpy(ret->nome, (const char *)nome);
            }
            else
            {
                funcionario_close(&ret);
            }
        }
    }

    return ret;
}

funcionario_retorno_typedef funcionario_init_estatico(funcionario_t *obj, funcionario_contrato_typedef tipo_contrato, funcionario_tipo_contrato_t contrato, char *nome, uint32_t contratos_assinados, double comissao_por_contrato)
{
    funcionario_retorno_typedef ret = FUNCIONARIO_RETORNO_FAIL;

    // Verifica se a qtde de bytes - incluindo o '\0' - para o nome nao da overflow no buffer
    if ((strlen((const char *)obj->nome) + 1) <= FUNCIONARIO_LEN_NOME)
    {
        obj->tipo_contrato = tipo_contrato;
        obj->contratos_assinados = contratos_assinados;
        obj->comissao_por_contrato = comissao_por_contrato;

        // Realiza as atribuicoes de acordo com o tipo de contrato  e verifica se a rotina retornou falha
        if (funcionario_set_contrato_callback[tipo_contrato](obj, &contrato) == FUNCIONARIO_RETORNO_OK)
        {
            strcpy(obj->nome, (const char *)nome);
            ret = FUNCIONARIO_RETORNO_OK;
        }
        else
        {
            // Caso o metodo de definicao de contrato encontre alguma inconsistência o presente objeto sera desconsiderdo
            funcionario_set_invalid(obj);
        }
    }

    return ret;
}

funcionario_retorno_typedef funcionario_close(funcionario_t **obj)
{

    if (obj != NULL && *obj != NULL)
    {
        free(*obj);
        *obj = NULL;
    }

    return FUNCIONARIO_RETORNO_OK;
}

funcionario_retorno_typedef funcionario_close_all(funcionario_t ***obj, uint32_t length)
{
    register int i;

    // Permite modificar o que tem no endereco do primeiro endereco do array
    funcionario_t **aux = *(&obj[0]);

    if (obj != NULL && *obj != NULL)
    {
        for (i = 0; i < length; ++i)
            funcionario_close(&aux[i]);

        free(*obj);
        *obj = NULL;
    }

    return FUNCIONARIO_RETORNO_OK;
}

double funcionario_calcular_total_pagar_funcionario(funcionario_t *obj)
{
    double ret = 0;

    if (funcionario_is_valid(obj))
        ret = (obj->comissao_por_contrato * obj->contratos_assinados) + funcionario_calcula_pagamento_callback[obj->tipo_contrato](obj);

    return ret;
}

void funcionario_imprime_funcionarios_dinamico(funcionario_t **obj, uint32_t lenght)
{
    register int i;

    for (i = 0; i < lenght; ++i)
    {
        if (funcionario_is_valid(obj[i]))
        {
            printf("----------------------------\n");
            printf("Nome: %s\n", obj[i]->nome);
            printf("Contrato: %s\n", FUNCIONARIO_TIPO_CONTRATO[obj[i]->tipo_contrato]);

            switch (obj[i]->tipo_contrato)
            {
            case FUNCIONARIO_CONTRATO_CLT:
                printf("Salario: %.2lf\n", obj[i]->contrato.clt.salario);
                break;
            case FUNCIONARIO_CONTRATO_TERCEIRIZADO:
                printf("Valor por hora: %.2lf\n", obj[i]->contrato.terceirizado.valor_por_hora);
                printf("Horas trabalhadas: %d\n", obj[i]->contrato.terceirizado.horas_trabalhadas);
                break;

            default:
                break;
            }

            printf("Contratos assinados: %d\n", obj[i]->contratos_assinados);
            printf("Comissao por contrato: %.2lf\n", obj[i]->comissao_por_contrato);
            printf("Total a receber = %.2lf\n", funcionario_calcular_total_pagar_funcionario(obj[i]));
            printf("----------------------------\n");
        }
    }
}

void funcionario_imprime_funcionarios_estatico(funcionario_t *obj, uint32_t lenght)
{
    register int i;

    for (i = 0; i < lenght; ++i)
    {
        if (funcionario_is_valid(&obj[i]))
        {
            printf("----------------------------\n");
            printf("Nome: %s\n", obj[i].nome);
            printf("Contrato: %s\n", FUNCIONARIO_TIPO_CONTRATO[obj[i].tipo_contrato]);

            switch (obj[i].tipo_contrato)
            {
            case FUNCIONARIO_CONTRATO_CLT:
                printf("Salario: %.2lf\n", obj[i].contrato.clt.salario);
                break;
            case FUNCIONARIO_CONTRATO_TERCEIRIZADO:
                printf("Valor por hora: %.2lf\n", obj[i].contrato.terceirizado.valor_por_hora);
                printf("Horas trabalhadas: %d\n", obj[i].contrato.terceirizado.horas_trabalhadas);
                break;

            default:
                break;
            }

            printf("Contratos assinados: %d\n", obj[i].contratos_assinados);
            printf("Comissao por contrato: %.2lf\n", obj[i].comissao_por_contrato);
            printf("Total a receber = %.2lf\n", funcionario_calcular_total_pagar_funcionario(&obj[i]));
            printf("----------------------------\n");
        }
    }
}

double funcionario_calcular_folha_pagamento_dinamico(funcionario_t **obj, uint32_t lenght)
{
    register int i;
    double ret = 0;

    if (obj != NULL)
    {
        for (i = 0; i < lenght; ++i)
        {
            if (funcionario_is_valid(obj[i]))
                ret += funcionario_calcular_total_pagar_funcionario(obj[i]);
        }
    }

    return ret;
}

double funcionario_calcular_folha_pagamento_estatico(funcionario_t *obj, uint32_t lenght)
{
    register int i;
    double ret = 0;

    if (obj != NULL)
    {
        for (i = 0; i < lenght; ++i)
        {
            if (funcionario_is_valid(&obj[i]))
                ret += funcionario_calcular_total_pagar_funcionario(&obj[i]);
        }
    }

    return ret;
}

// -------------------------------------------------------
// Implementacao dos Metodos privados
// -------------------------------------------------------
static bool funcionario_is_valid(funcionario_t *obj)
{
    bool ret = false;

    if (obj != NULL && obj->nome[0] != FUNCIONARIO_CARACTERE_DE_INVALIDO)
        ret = true;

    return ret;
}

static funcionario_retorno_typedef funcionario_set_invalid(funcionario_t *obj)
{
    funcionario_retorno_typedef ret = FUNCIONARIO_RETORNO_FAIL;

    if (obj != NULL)
    {
        obj->nome[0] = FUNCIONARIO_CARACTERE_DE_INVALIDO;
        ret = FUNCIONARIO_RETORNO_OK;
    }

    return ret;
}

static funcionario_retorno_typedef funcionario_set_clt(funcionario_t *func_obj, funcionario_tipo_contrato_t *clt_obj)
{
    funcionario_retorno_typedef ret = FUNCIONARIO_RETORNO_FAIL;

    // Verifca se as referencias nos ponteiros sao validas
    if (func_obj != NULL && clt_obj != NULL)
    {
        // Verifica se o tipo de contrato eh valido (CONSIDERADO UM VALOR MINIMO PARA SALARIO PARA EVITAR QUE SEJA PASSADO A ESTRUTURA ERRADA EM FUNCAO DO TIPO DE CONTRATO DEFINIDO)
        if (func_obj->tipo_contrato == FUNCIONARIO_CONTRATO_CLT && clt_obj->clt.salario >= FUNCIONARIO_MIN_SALARIO)
        {
            func_obj->contrato.clt.salario = clt_obj->clt.salario;
            ret = FUNCIONARIO_RETORNO_OK;
        }
    }

    return ret;
}

static funcionario_retorno_typedef funcionario_set_terceirizado(funcionario_t *func_obj, funcionario_tipo_contrato_t *terc_obj)
{
    funcionario_retorno_typedef ret = FUNCIONARIO_RETORNO_FAIL;

    // Verifca se as referencias nos ponteiros sao validas
    if (func_obj != NULL && terc_obj != NULL)
    {
        // Verifica se o tipo de contrato eh valido (CONSIDERADO UM VALOR MINIMO PARA VALOR_HORA PARA EVITAR QUE SEJA PASSADO A ESTRUTURA ERRADA EM FUNCAO DO TIPO DE CONTRATO DEFINIDO)
        if (func_obj->tipo_contrato == FUNCIONARIO_CONTRATO_TERCEIRIZADO && terc_obj->terceirizado.valor_por_hora >= FUNCIONARIO_MIN_VALOR_HORA)
        {
            func_obj->contrato.terceirizado.horas_trabalhadas = terc_obj->terceirizado.horas_trabalhadas;
            func_obj->contrato.terceirizado.valor_por_hora = terc_obj->terceirizado.valor_por_hora;
            ret = FUNCIONARIO_RETORNO_OK;
        }
    }

    return ret;
}

static double funcionario_calcula_pagamento_clt(funcionario_t *obj)
{
    double ret = 0;

    if (obj != NULL)
        ret = obj->contrato.clt.salario;

    return ret;
}

static double funcionario_calcula_pagamento_terceirizado(funcionario_t *obj)
{
    double ret = 0;

    if (obj != NULL)
        ret = (obj->contrato.terceirizado.horas_trabalhadas * obj->contrato.terceirizado.valor_por_hora);

    return ret;
}
