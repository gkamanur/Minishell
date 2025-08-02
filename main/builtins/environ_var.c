/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:51:09 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/02 14:14:09 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env *create_env_node(const char *key, const char *value) {
    t_env *node = malloc(sizeof(t_env));
    if (!node) return NULL;
    node->key = strdup(key);
    node->value = strdup(value);
    if (!node->key || !node->value) {
        free(node->key);
        free(node->value);
        free(node);
        return NULL;
    }
    node->next = NULL;
    return node;
}

t_env *init_env(char **envp) {
    t_env *env_list = NULL;
    for (int i = 0; envp[i]; i++) {
        char *entry = strdup(envp[i]);
        if (!entry) continue;
        char *eq = strchr(entry, '=');
        if (eq) {
            *eq = '\0';
            t_env *node = create_env_node(entry, eq + 1);
            if (node) {
                node->next = env_list;
                env_list = node;
            }
        }
        free(entry);
    }
    return env_list;
}

char *get_env_value(const char *key, t_env *env_list) {
    t_env *current = env_list;
    while (current) {
        if (strcmp(current->key, key) == 0)
            return current->value;
        current = current->next;
    }
    return "";
}

void free_env(t_env *env_list) {
    while (env_list) {
        t_env *tmp = env_list;
        env_list = env_list->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}

