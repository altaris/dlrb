#include <assert.h>
#include <concord/discord.h>
#include <concord/log.h>
#include <stdio.h>
#include <string.h>

struct {
  u64snowflake channel_id;
  u64snowflake message_id;
} last_message = {
    .channel_id = 0,
    .message_id = 0,
};

const char* emojis[] = {
    "🇦", "🇧", "🇨", "🇩", "🇪", "🇫", "🇬", "🇭", "🇮", "🇯", "🇰", "🇱",
    "🇲", "🇳", "🇴", "🇵", "🇶", "🇷", "🇸", "🇹", "🇺", "🇻", "🇼", "🇽",
    "🇾", "🇿", "0️⃣", "1️⃣", "2️⃣", "3️⃣", "4️⃣", "5️⃣", "6️⃣", "7️⃣", "8️⃣", "9️⃣",
    // "❗️", "❓"
};

void on_command(struct discord* client, const struct discord_message* msg) {
  if (msg->author->bot) return;
  struct discord_delete_message dp = {.reason = "delete /lr cmd"};
  discord_delete_message(client, msg->channel_id, msg->id, &dp, NULL);
  if (last_message.channel_id != 0) {
    for (int i = 0; msg->content[i] != '\0'; i++) {
      char c = msg->content[i];
      if ('0' <= c && c <= '9') {
        discord_create_reaction(client, last_message.channel_id,
                                last_message.message_id, 0,
                                emojis[26 + c - '0'], NULL);
      } else if ('A' <= c && c <= 'Z') {
        discord_create_reaction(client, last_message.channel_id,
                                last_message.message_id, 0, emojis[c - 'A'],
                                NULL);
      } else if ('a' <= c && c <= 'z') {
        discord_create_reaction(client, last_message.channel_id,
                                last_message.message_id, 0, emojis[c - 'a'],
                                NULL);
      } else if (c == '!') {
        discord_delete_all_reactions(client, last_message.channel_id,
                                     last_message.message_id, NULL);
      }
    }
  }
}

void on_message(struct discord* client, const struct discord_message* msg) {
  if (msg->author->bot || strncmp(msg->content, "/lr", 3) == 0) return;
  last_message.channel_id = msg->channel_id;
  last_message.message_id = msg->id;
}

int main(int argc, char* argv[]) {
  assert(argc > 1);
  ccord_global_init();
  struct discord* client = discord_config_init(argv[1]);
  assert(NULL != client && "Couldn't initialize client");
  discord_add_intents(client, DISCORD_GATEWAY_MESSAGE_CONTENT);
  discord_set_on_command(client, "/lr", &on_command);
  discord_set_on_message_create(client, &on_message);
  return discord_run(client);
}