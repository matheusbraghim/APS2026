import os
import numpy as np
import matplotlib.pyplot as plt

def carregar_dados(caminho_arquivo):
    """
    Lê o arquivo gerado pelo programa em C/C++.
    O arquivo deve ter o formato: n,mediana_ciclos
    """

    if not os.path.exists(caminho_arquivo):
        raise FileNotFoundError(f"Arquivo {caminho_arquivo} não encontrado.")

    # Carrega os dados ignorando linhas de comentário (#)
    dados = np.loadtxt(caminho_arquivo, delimiter=',', skiprows=1)

    n = dados[:, 0].astype(float)

    series = {
        'Lista não ordenada': {
            'Inserção': dados[:, 1].astype(float),
            'Remoção': dados[:, 2].astype(float),
            'Listagem ordenada': dados[:, 3].astype(float)
        },
        'Lista ordenada': {
            'Inserção': dados[:, 4].astype(float),
            'Remoção': dados[:, 5].astype(float),
            'Listagem ordenada': dados[:, 6].astype(float)
        },
        'Árvore binária': {
            'Inserção': dados[:, 7].astype(float),
            'Remoção': dados[:, 8].astype(float),
            'Listagem ordenada': dados[:, 9].astype(float)
        }
    }

    return n, series


def ajustar_lei_potencia(n, y):
    """
    Método 1: Ajuste log-log para estimar o expoente k (y = c * n^k).
    Útil para identificar a ordem de grandeza inicial.
    """

    mascara = (n > 0) & (y > 0)
    n = n[mascara]
    y = y[mascara]

    if len(n) < 2:
        return np.nan, np.nan

    log_n = np.log(n)
    log_y = np.log(y)

    coef = np.polyfit(log_n, log_y, deg=1)

    k = coef[0]
    c = np.exp(coef[1])

    return k, c


def ajustar_modelo(n, y, f_n):
    """
    Método 2: Mínimos quadrados para a função f(n) específica.
    Calcula o coeficiente de determinação R2.
    """

    # c = Σ(y * f(n)) / Σ(f(n)^2)
    c = np.sum(y * f_n) / np.sum(f_n ** 2)
    y_previsto = c * f_n

    # Cálculo do R2
    ss_res = np.sum((y - y_previsto) ** 2)
    ss_tot = np.sum((y - np.mean(y)) ** 2)
    r2 = 1.0 - (ss_res / ss_tot) if ss_tot > 0 else 0.0

    return c, r2, y_previsto


def analisar_complexidade(n, y):
    """
    Testa as complexidades candidatas e retorna os resultados.
    """

    modelos = {
        'O(1)': np.ones_like(n),
        'O(log n)': np.log(n),
        'O(n)': n,
        'O(n log n)': n * np.log(n),
        'O(n^1.5)': n ** 1.5,
        'O(n^2)': n ** 2,
        'O(n^3)': n ** 3
    }

    resultados = {}

    for nome, f_n in modelos.items():
        c, r2, y_previsto = ajustar_modelo(n, y, f_n)
        resultados[nome] = {'c': c, 'r2': r2, 'previsto': y_previsto}

    # Define o melhor modelo baseado no maior R2
    melhor_nome = max(resultados, key=lambda k: resultados[k]['r2'])

    return resultados, melhor_nome


def plotar_resultados(n, series, resultados, titulo_grafico):
    """
    Gera o gráfico comparativo.
    """

    plt.figure(figsize=(10, 6))

    for estrutura, y in series.items():

        resultado = resultados[estrutura]

        # Pontos experimentais
        plt.scatter(n, y, label=f'{estrutura} - Medições (Mediana)', zorder=5)

        # Curva de ajuste do melhor modelo
        # Criamos um n_linha suave para a curva não ficar "quebrada"
        n_linha = np.linspace(min(n), max(n), 500)

        # Define a função para o n_linha baseada no melhor modelo
        melhor_nome = resultado['melhor']

        if melhor_nome == 'O(1)': f_linha = np.ones_like(n_linha)
        elif melhor_nome == 'O(log n)': f_linha = np.log(n_linha)
        elif melhor_nome == 'O(n)': f_linha = n_linha
        elif melhor_nome == 'O(n log n)': f_linha = n_linha * np.log(n_linha)
        elif melhor_nome == 'O(n^1.5)': f_linha = n_linha ** 1.5
        elif melhor_nome == 'O(n^2)': f_linha = n_linha ** 2
        elif melhor_nome == 'O(n^3)': f_linha = n_linha ** 3

        c_otimo = resultado['resultados'][melhor_nome]['c']

        plt.plot(
            n_linha,
            c_otimo * f_linha,
            linestyle='--',
            label=f'{estrutura} - Ajuste: {melhor_nome} (R²={resultado["resultados"][melhor_nome]["r2"]:.4f})'
        )

    plt.title(f"Análise da escalabilidade assintótica {titulo_grafico}")
    plt.xlabel("Tamanho da Entrada (n)")
    plt.ylabel("Tempo (segundos)")
    plt.legend()
    plt.xscale('log')
    plt.yscale('log')
    plt.xticks(n, [f'{int(valor):,}'.replace(',', '.') for valor in n])

    from matplotlib.ticker import FuncFormatter
    plt.gca().yaxis.set_major_formatter(
        FuncFormatter(lambda valor, pos: f'{valor:g}')
    )
    plt.grid(True, linestyle=':', alpha=0.6)
    plt.show()


# --- EXECUÇÃO PRINCIPAL ---
if __name__ == "__main__":

    # Nome do arquivo gerado pelo C/C++
    NOME_ARQUIVO = os.path.join("resultados", "resultados.csv")

    try:
        n, series = carregar_dados(NOME_ARQUIVO)

        # --- ALTERNATIVA: CARREGAMENTO MANUAL DE DADOS PARA TESTE ---
        # Comente a linha: n, y = carregar_dados(NOME_ARQUIVO)
        # E utilize um dos blocos abaixo para testar o fitting:

        # Exemplo 1: Simulação de comportamento Linear O(n)
        # n = np.array([1000, 5000, 10000, 50000, 100000])
        # y = np.array([1200, 5100, 10500, 50200, 100800])

        # Exemplo 2: Simulação de comportamento Logarítmico O(log n)
        # n = np.array([1000, 5000, 10000, 50000, 100000])
        # y = np.array([10, 12, 13, 15, 17])

        # Exemplo 3: Simulação de comportamento Quadrático O(n^2)
        # n = np.array([1000, 5000, 10000, 50000, 100000])
        # y = np.array([1000000, 25000000, 100000000, 2500000000, 10000000000])

        # Exemplo 4: Simulação de O(n log n) - Comum em ordenação
        # n = np.array([1000, 5000, 10000, 50000, 100000])
        # y = n * np.log(n) * np.random.uniform(0.99,1.2,5) # (Simulado com um ruído de 20%)
        # -----------------------------------------------------------

        operacoes = ['Inserção', 'Remoção', 'Listagem ordenada']

        for operacao in operacoes:

            series_operacao = {
                estrutura: series[estrutura][operacao]
                for estrutura in series
            }

            resultados_operacao = {}

            for estrutura, y in series_operacao.items():

                # Passo 1: Lei de Potência
                k_exp, c_pot = ajustar_lei_potencia(n, y)
                print(f"\n{estrutura} - {operacao}")
                print(f"Expoente k estimado (log-log): {k_exp:.4f}")

                # Passo 2: Teste de classes específicas
                resultados, melhor = analisar_complexidade(n, y)

                print("\nResultados por classe de complexidade:")
                for classe, res in resultados.items():
                    print(f" {classe:10} -> R² = {res['r2']:.6f}")

                print(f"\nConclusão Experimental: A estrutura comporta-se como {melhor}")

                resultados_operacao[estrutura] = {
                    'resultados': resultados,
                    'melhor': melhor
                }

            # Passo 3: Visualização
            plotar_resultados(
                n,
                series_operacao,
                resultados_operacao,
                operacao
            )

        series_custo = {}

        for estrutura in series:

            y = (
                0.70 * series[estrutura]['Inserção']
                + 0.15 * series[estrutura]['Remoção']
                + 0.15 * series[estrutura]['Listagem ordenada']
            )

            series_custo[estrutura] = y

        resultados_custo = {}

        for estrutura, y in series_custo.items():

            # Passo 1: Lei de Potência
            k_exp, c_pot = ajustar_lei_potencia(n, y)
            print(f"\n{estrutura} - Custo total ponderado")
            print(f"Expoente k estimado (log-log): {k_exp:.4f}")

            # Passo 2: Teste de classes específicas
            resultados, melhor = analisar_complexidade(n, y)

            print("\nResultados por classe de complexidade:")
            for classe, res in resultados.items():
                print(f" {classe:10} -> R² = {res['r2']:.6f}")

            print(f"\nConclusão Experimental: A estrutura comporta-se como {melhor}")

            resultados_custo[estrutura] = {
                'resultados': resultados,
                'melhor': melhor
            }

        # Passo 3: Visualização
        plotar_resultados(
            n,
            series_custo,
            resultados_custo,
            "Custo Total Ponderado"
        )

    except Exception as e:
        print(f"Erro: {e}")